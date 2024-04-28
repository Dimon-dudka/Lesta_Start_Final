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

	GrenadeComponent = CreateDefaultSubobject<UGrenadeShootComponent>(TEXT("Grenade Component"));
	GrenadeComponent->EndOfExpl.AddDynamic(this, &AEnemyActorBasic::GetDestroyed);

	FollowingComp = CreateDefaultSubobject<UFollowingPlayerComponent>(TEXT("Following Component"));
	FollowingComp->ExplosionStart.AddDynamic(this, &AEnemyActorBasic::GetNullHPInfo);
	//FollowingComp->ExplosionStart.AddDynamic(GrenadeComp, &UGrenadeShootComponent::StartShoot);
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
	Mesh->DestroyComponent();

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

	PrintHP->SetupHealthPoints(Health->GetHP());
	GuardComp->InitialSetup(GetActorLocation());

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