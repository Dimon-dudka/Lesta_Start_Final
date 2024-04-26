#include "EnemyActorBasic.h"

AEnemyActorBasic::AEnemyActorBasic()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DestroyComp = CreateDefaultSubobject<UDestroyAnimationComponent>(TEXT("Dstroy Animation"));
	DestroyComp->SetupAttachment(RootComponent);
	DestroyComp->AnimationStatus.AddDynamic(this, &AEnemyActorBasic::GetDestroyed);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->GetNullHP.AddDynamic(this,&AEnemyActorBasic::GetNullHPInfo);

	PrintHP = CreateDefaultSubobject<UHPPrintComponent>(TEXT("HP Print"));
	PrintHP->SetupAttachment(RootComponent);
	PrintHP->SetupHealthPoints(Health->GetHP());

	GuardComp = CreateDefaultSubobject<UEnemyGuardComonent>(TEXT("Guardian Behavior"));
	GuardComp->NewPosDelegate.AddDynamic(this, &AEnemyActorBasic::GetNewLocation);

	GrenadeComp = CreateDefaultSubobject<UGrenadeEnemyComponent>(TEXT("Grenade Component"));

	FollowingComp = CreateDefaultSubobject<UFollowingPlayerComponent>(TEXT("Following Component"));
	FollowingComp->ExplosionStart.AddDynamic(GrenadeComp, &UGrenadeEnemyComponent::StartExplosion);
	FollowingComp->ExplosionStart.AddDynamic(this, &AEnemyActorBasic::GetNullHPInfo);

	LazerShoot = CreateDefaultSubobject<UShootLaserComponent>(TEXT("Lazer Shoot"));

	Tracer = CreateDefaultSubobject<UTraceComponent>(TEXT("Line Trace"));
	Tracer->TraceResult.AddDynamic(LazerShoot, &UShootLaserComponent::Shoot);
	Tracer->TraceResult.AddDynamic(GuardComp, &UEnemyGuardComonent::GetHitRes);
	Tracer->TraceResult.AddDynamic(FollowingComp, &UFollowingPlayerComponent::GetTargetHit);
}

void AEnemyActorBasic::GetNewLocation(FVector NewLocation) {
	SetActorLocation(NewLocation);
}

void AEnemyActorBasic::GetDestroyed() {
	Destroy();
}

void AEnemyActorBasic::GetDamage(const double& Damage) {
	Health->BecomeDamage(Damage);
	PrintHP->SetupHealthPoints(Health->GetHP()<0.0?0: Health->GetHP());
}

void AEnemyActorBasic::GetNullHPInfo() {
	LazerShoot->DestroyComponent();
	PrintHP->DestroyComponent();
	DestroyComp->StartAnimation();
	GrenadeComp->StartExplosion();
	Mesh->DestroyComponent();
}

void AEnemyActorBasic::BeginPlay()
{
	Super::BeginPlay();

	PrintHP->SetupHealthPoints(Health->GetHP());
	GuardComp->InitialSetup(GetActorLocation());

	if (!GuardComp->IsThisEnemyGuard) {
		FollowingComp->NewPosDelegate.AddDynamic(this, &AEnemyActorBasic::GetNewLocation);
	}

	FollowingComp->SetFollowingPlayer(GrenadeComp->FlagIsGrenade);
	FollowingComp->InitStartPosition(GetActorLocation());
}

void AEnemyActorBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}