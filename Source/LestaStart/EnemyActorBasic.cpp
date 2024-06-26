#include "EnemyActorBasic.h"
#include "Net/UnrealNetwork.h"

AEnemyActorBasic::AEnemyActorBasic()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DestroyComp = CreateDefaultSubobject<UDestroyAnimationComponent>(TEXT("Dstroy Animation"));
	DestroyComp->SetupAttachment(RootComponent);
	DestroyComp->AnimationStatus.AddDynamic(this, &AEnemyActorBasic::GetDestroyed);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->GetNullHP.AddDynamic(this,&AEnemyActorBasic::GetNullHPInfo);

	Health->GetHPValue.AddDynamic(this, &AEnemyActorBasic::ChangeHP);

	PrintHP = CreateDefaultSubobject<UHPPrintComponent>(TEXT("HP Print"));
	PrintHP->SetupAttachment(RootComponent);
	PrintHP->SetupHealthPoints(Health->GetHP());

	GuardComp = CreateDefaultSubobject<UEnemyGuardComonent>(TEXT("Guardian Behavior"));
	GuardComp->NewPosDelegate.AddDynamic(this, &AEnemyActorBasic::GetNewLocation);

	GrenadeComponent = CreateDefaultSubobject<UGrenadeShootComponent>(TEXT("Grenade Component"));
	GrenadeComponent->EndOfExpl.AddDynamic(this, &AEnemyActorBasic::GetDestroyed);

	FollowingComp = CreateDefaultSubobject<UFollowingPlayerComponent>(TEXT("Following Component"));
	FollowingComp->ExplosionStart.AddDynamic(this, &AEnemyActorBasic::GetNullHPInfo);
	FollowingComp->ExplosionStart.AddDynamic(this, &AEnemyActorBasic::GetNullHPInfo);

	LazerShootComp = CreateDefaultSubobject<ULazerShootComponent>(TEXT("Lazer Component"));

	TraceComp = CreateDefaultSubobject<UTraceEnemiesComponent>(TEXT("Trace Component"));
	TraceComp->TraceResult.AddDynamic(LazerShootComp, &ULazerShootComponent::GetHit);
	TraceComp->TraceResult.AddDynamic(GuardComp, &UEnemyGuardComonent::GetHitRes);
	TraceComp->TraceResult.AddDynamic(FollowingComp, &UFollowingPlayerComponent::GetTargetHit);

	this->ShootStatus.AddDynamic(LazerShootComp, &ULazerShootComponent::ShootStatus);
	this->ChangeHPDelegate.AddDynamic(PrintHP, &UHPPrintComponent::SetupHealthPoints);
}

void AEnemyActorBasic::ChangeHP_Implementation(double HP) {
	if (ChangeHPDelegate.IsBound()) {
		ChangeHPDelegate.Broadcast(HP);
	}
}


void AEnemyActorBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const  {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyActorBasic, FollowingComp);
	DOREPLIFETIME(AEnemyActorBasic, GrenadeComponent);
	DOREPLIFETIME(AEnemyActorBasic, GuardComp);
	DOREPLIFETIME(AEnemyActorBasic, DestroyComp);
	DOREPLIFETIME(AEnemyActorBasic, Health);
	DOREPLIFETIME(AEnemyActorBasic, Box);
	DOREPLIFETIME(AEnemyActorBasic, LazerShootComp);
	DOREPLIFETIME(AEnemyActorBasic, TraceComp);
	DOREPLIFETIME(AEnemyActorBasic, Mesh);
}

void AEnemyActorBasic::GetNewLocation_Implementation(FVector NewLocation) {
	SetActorLocation(NewLocation);
}

void AEnemyActorBasic::GetDestroyed_Implementation() {
	Destroy();
}

void AEnemyActorBasic::GetDamage(const double& Damage) {

	if (!HasAuthority())return;

	Health->BecomeDamage(Damage);
}

void AEnemyActorBasic::GetNullHPInfo_Implementation() {
	if (LazerShootComp)LazerShootComp->DestroyComponent();
	if (PrintHP)PrintHP->DestroyComponent();
	if (Mesh)Mesh->DestroyComponent();

	if (!GrenadeComponent->IsGrenade) {
		DestroyComp->StartAnimation();
	}
	else {
		GrenadeComponent->StartShoot(1.0, GetActorLocation());
	}
}

void AEnemyActorBasic::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetIsReplicated(true);

	PrintHP->SetupHealthPoints(Health->GetHP());
	GuardComp->InitialSetup(GetActorLocation());

	if (ShootStatus.IsBound()) {
		ShootStatus.Broadcast(1);
	}

	if (!GuardComp->IsThisEnemyGuard) {
		FollowingComp->NewPosDelegate.AddDynamic(this, &AEnemyActorBasic::GetNewLocation);
	}

	FollowingComp->SetFollowingPlayer(GrenadeComponent->IsGrenade);
	FollowingComp->InitStartPosition(GetActorLocation());
}

void AEnemyActorBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}