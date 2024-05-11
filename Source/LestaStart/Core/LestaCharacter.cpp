#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

#include "../PlayerHUD.h"

ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 20.f;
	
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	ChoisenWeapon = WEAPON_TYPE::LAZER;
	IsShooting = false;
	IsReloading = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->GetNullHP.AddDynamic(this, &ALestaCharacter::KillPlayer);
	HealthComponent->GetHPValue.AddDynamic(this, &ALestaCharacter::ChangeHPHUD);

	GrenadeComponent = CreateDefaultSubobject<UGrenadeShootComponent>(TEXT("Grenade Component"));
	GrenadeComponent->IsReload.AddDynamic(this, &ALestaCharacter::EndOfReload);

	LazerComponent = CreateDefaultSubobject<ULazerShootComponent>(TEXT("Lazer Component"));
	LazerComponent->IsReload.AddDynamic(this, &ALestaCharacter::EndOfReload);

	TraceComp = CreateDefaultSubobject<UTracePlayersComponent>(TEXT("Trace Component"));
	TraceComp->HitDelegate.AddDynamic(LazerComponent, &ULazerShootComponent::GetHit);

	PrintHP = CreateDefaultSubobject<UHPPrintComponent>(TEXT("HP Print Component"));
	PrintHP->SetupAttachment(RootComponent);
	PrintHP->SetupHealthPoints(HealthComponent->GetHP());

	this->ShootingStatus.AddDynamic(LazerComponent, &ULazerShootComponent::ShootStatus);
	this->StartReload.AddDynamic(LazerComponent, &ULazerShootComponent::StartReload);
	this->ChangeHP.AddDynamic(PrintHP, &UHPPrintComponent::SetupHealthPoints);
}

void ALestaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALestaCharacter, ChoisenWeapon);
	DOREPLIFETIME(ALestaCharacter, IsShooting);
	DOREPLIFETIME(ALestaCharacter, IsReloading);
	DOREPLIFETIME(ALestaCharacter, GrenadeTimeCount)
	DOREPLIFETIME(ALestaCharacter, StartReload);
	DOREPLIFETIME(ALestaCharacter, ShootingStatus);
	DOREPLIFETIME(ALestaCharacter, LazerComponent);
	DOREPLIFETIME(ALestaCharacter, HealthComponent);
	DOREPLIFETIME(ALestaCharacter, TraceComp);
	DOREPLIFETIME(ALestaCharacter, GrenadeComponent);
}

void ALestaCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PC = GetController<APlayerController>()) {

		//	Input System Setup
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer()) {
			if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				InputSystem->AddMappingContext(IMC, 0);
			}
		}
		
		//	HUD Setup
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
			APlayerController* PlayerController = It->Get();
			if (PlayerController==Cast<APlayerController>(this->GetController())) {

				UserPlayerController = PlayerController;

				APlayerHUD* UserHUD = GetWorld()->SpawnActor<APlayerHUD>();
				PlayerController->MyHUD = UserHUD;
				
				ChangeHP.AddDynamic(UserHUD, &APlayerHUD::SetTextHP);
				ChangeWeapon.AddDynamic(UserHUD, &APlayerHUD::SetTextWeapon);
				Reloading.AddDynamic(UserHUD, &APlayerHUD::SetReloadingText);
			}
		}

		//	Initial HUD textes setup
		if (ChangeHP.IsBound()&& ChangeWeapon.IsBound()&& Reloading.IsBound()) {
			ChangeHP.Broadcast(HealthComponent->GetHP());
			ChangeWeapon.Broadcast("Lazer");
			Reloading.Broadcast(false);
		}
	}
}

void ALestaCharacter::GetDamage(const double& Damage) {

	if (!HasAuthority())return;

	HealthComponent->BecomeDamage(Damage);
}

void ALestaCharacter::ChangeHPHUD_Implementation(double HP) {
	//	HUD Update
	if (ChangeHP.IsBound()) {
		ChangeHP.Broadcast(HP);
	}
}

void ALestaCharacter::KillPlayer_Implementation() {
	//	Restart Game

	if (!HasAuthority())return;

	Destroy();
}

void ALestaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC) // should validate component because developers can change input component class through Project Settings
	{

		EIC->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
		EIC->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookInput);

		EIC->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnShootInput);
		EIC->BindAction(ShootInputAction, ETriggerEvent::Ongoing, this, &ThisClass::OnShootInput);
		EIC->BindAction(ChangeWeaponInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnChangeWeaponInput);
		EIC->BindAction(ReloadInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnReloadInput);
	}
	else
	{
		// Print error message into log
		// You can read more here: https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}

void ALestaCharacter::OnShootInput(const FInputActionInstance& InputActionInstance) {

	//	If weapon is reloading user cannot shoot
	if (IsReloading) return;

	switch (InputActionInstance.GetTriggerEvent()) {

	case ETriggerEvent::Triggered:
		switch (ChoisenWeapon) {
		case WEAPON_TYPE::GRENADE:

			if (GrenadeTimeCount >= 0.1) {
				GrenadeComponent->StartShoot(GrenadeTimeCount, GetActorLocation());
			}

			GrenadeTimeCount = 0.0;
			break;

		case WEAPON_TYPE::LAZER:

			if (ShootingStatus.IsBound()) {
				ShootingStatus.Broadcast(0);
			}

			break;
		}

		IsShooting = false;
		break;

	case ETriggerEvent::Ongoing:

		IsShooting = true;

		switch (ChoisenWeapon) {

		case WEAPON_TYPE::GRENADE:
			if (GrenadeTimeCount < GrenadeComponent->GetGrenadeMaxTime()) {
				GrenadeTimeCount = InputActionInstance.GetElapsedTime() / GrenadeComponent->GetGrenadeMaxTime();
			}
			GrenadeComponent->GetGrenadeMaxTime() < GrenadeTimeCount ?
				GrenadeTimeCount = GrenadeComponent->GetGrenadeMaxTime() : GrenadeTimeCount;

			break;
		case WEAPON_TYPE::LAZER:
			
			if (ShootingStatus.IsBound()) {
				ShootingStatus.Broadcast(1);
			}

			break;
		}
		break;
	}
}

void ALestaCharacter::EndOfReload_Implementation() {
	IsReloading = false;

	//	HUD Text update
	if (Reloading.IsBound()) {
		Reloading.Broadcast(false);
	}
}

void ALestaCharacter::OnReloadInput(const FInputActionInstance& InputActionInstance) {

	if (IsShooting || IsReloading)return;

	IsReloading = true;
	if (Reloading.IsBound()) {
		//HUD Text Update
		Reloading.Broadcast(true);
	}

	if (ChoisenWeapon == WEAPON_TYPE::GRENADE) {
		GrenadeComponent->StartReload();
	}
	else if (ChoisenWeapon == WEAPON_TYPE::LAZER) {
		
		if (StartReload.IsBound()) {
			StartReload.Broadcast();
		}
	}
}

void ALestaCharacter::OnChangeWeaponInput(const FInputActionInstance& InputActionInstance) {

	if (IsShooting||IsReloading)return;

	double GettedValue = InputActionInstance.GetValue().Get<float>();

	//	Around value 1 - Key "1"
	if (GettedValue >= 0.5 && GettedValue <= 1.4) {
		ChoisenWeapon = WEAPON_TYPE::LAZER;
		if (ChangeWeapon.IsBound()) {
			ChangeWeapon.Broadcast("Lazer");
		}
	}
	//	Around value 2 - Key "2"
	else if (GettedValue >= 1.5 && GettedValue <= 2.4) {
		ChoisenWeapon = WEAPON_TYPE::GRENADE;
		if (ChangeWeapon.IsBound()) {
			ChangeWeapon.Broadcast("Grenade");
		}
	}
	//	Around value 3 - Mouse wheel
	else if (GettedValue >= 2.5) {
		if (ChoisenWeapon == WEAPON_TYPE::GRENADE) {
			ChoisenWeapon = WEAPON_TYPE::LAZER;
			if (ChangeWeapon.IsBound()) {
				ChangeWeapon.Broadcast("Lazer");
			}
		}
		else {
			ChoisenWeapon = WEAPON_TYPE::GRENADE;
			if (ChangeWeapon.IsBound()) {
				ChangeWeapon.Broadcast("Grenade");
			}
		}
	}
}

void ALestaCharacter::OnMoveInput(const FInputActionInstance& InputActionInstance)
{
	// Controller rotation Yaw determines which direction Character is facing
	// so MoveForward = along that direction AND MoveRight = to the right of that direction

	const float YawDegree = GetControlRotation().Yaw; // controller rotation Yaw (in degrees)
	const float YawRadian = FMath::DegreesToRadians(YawDegree); // controller rotation Yaw (in radians)
	const FVector ForwardDirection = FVector(FMath::Cos(YawRadian), FMath::Sin(YawRadian), 0.f);
	const FVector RightDirection = FVector(FMath::Cos(YawRadian + UE_HALF_PI), FMath::Sin(YawRadian + UE_HALF_PI), 0.f);

	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddMovementInput(ForwardDirection * Input2D.X + RightDirection * Input2D.Y);
}

void ALestaCharacter::OnLookInput(const FInputActionInstance& InputActionInstance)
{
	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Input2D.X);
	AddControllerPitchInput(Input2D.Y);
}