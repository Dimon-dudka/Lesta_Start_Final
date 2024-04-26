#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"

#include "../PlayerHUD.h"

ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 10.f;

	ChoisenWeapon = WEAPON_TYPE::LAZER;
	IsShooting = false;
	IsReloading = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComp->GetNullHP.AddDynamic(this, &ALestaCharacter::KillPlayer);

	GrenadeComp = CreateDefaultSubobject<UGrenadeShootComponent>(TEXT("Grenade"));
	GrenadeComp->IsReload.AddDynamic(this, &ALestaCharacter::EndOfReload);

	LazerComp = CreateDefaultSubobject<ULazerShootUserComponent>(TEXT("Lazer"));
	LazerComp->IsReload.AddDynamic(this, &ALestaCharacter::EndOfReload);
	
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
		if (APlayerHUD* HUD = Cast<APlayerHUD>(PC->MyHUD.Get())) {
			ChangeHP.AddDynamic(HUD, &APlayerHUD::SetTextHP);
			ChangeWeapon.AddDynamic(HUD, &APlayerHUD::SetTextWeapon);
			Reloading.AddDynamic(HUD, &APlayerHUD::SetReloadingText);
		}
		
		//	Initial HUD textes setup
		if (ChangeHP.IsBound()&& ChangeWeapon.IsBound()&& Reloading.IsBound()) {
			ChangeHP.Broadcast(HealthComp->GetHP());
			ChangeWeapon.Broadcast("Lazer");
			Reloading.Broadcast(false);
		}
	}
}

void ALestaCharacter::GetDamage(const double& DamageHP) {
	HealthComp->BecomeDamage(DamageHP);

	//	HUD Update
	if (ChangeHP.IsBound()) {
		ChangeHP.Broadcast(HealthComp->GetHP());
	}
}

void ALestaCharacter::KillPlayer() {
	//	Restart Game
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
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
		EIC->BindAction(PlaceDefenceAction, ETriggerEvent::Triggered, this, &ThisClass::OnPlaceDefenceInput);

	}
	else
	{
		// Print error message into log
		// You can read more here: https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}

void ALestaCharacter::OnPlaceDefenceInput(const FInputActionInstance& InputActionInstance) {

}

void ALestaCharacter::OnShootInput(const FInputActionInstance& InputActionInstance) {

	//	If weapon is reloading user cannot shoot
	if (IsReloading) return;

	switch (InputActionInstance.GetTriggerEvent()) {

	case ETriggerEvent::Triggered:
		switch (ChoisenWeapon) {
		case WEAPON_TYPE::GRENADE:

			if (GrenadeTimeCount >= 0.1) {
				GrenadeComp->StartShoot(GrenadeTimeCount, GetActorLocation());
			}

			GrenadeTimeCount = 0.0;
			break;

		case WEAPON_TYPE::LAZER:
			LazerComp->GetShooting(0);
			break;
		}

		IsShooting = false;
		break;

	case ETriggerEvent::Ongoing:

		IsShooting = true;

		switch (ChoisenWeapon) {

		case WEAPON_TYPE::GRENADE:
			if (GrenadeTimeCount < GrenadeComp->GetGrenadeMaxTime()) {
				GrenadeTimeCount = InputActionInstance.GetElapsedTime()/ GrenadeComp->GetGrenadeMaxTime();
			}
			GrenadeComp->GetGrenadeMaxTime() < GrenadeTimeCount ?
				GrenadeTimeCount = GrenadeComp->GetGrenadeMaxTime() : GrenadeTimeCount;

			break;
		case WEAPON_TYPE::LAZER:
			LazerComp->GetShooting(1);
			break;
		}
		break;
	}
}

void ALestaCharacter::EndOfReload() {
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
		GrenadeComp->StartReload();
	}
	else if (ChoisenWeapon == WEAPON_TYPE::LAZER) {
		LazerComp->StartReload();
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