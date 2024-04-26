#include "GrenadeShootComponent.h"

UGrenadeShootComponent::UGrenadeShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Damage = 50.0;
	MaxDamageLen = 700.0;
	GreandeMaxTime = 1.0;

	GrenadesCountUsed = 0;
	GrenadesCount = 3;
	ReloadingFlag = false;
	CurrentReloadingTime = 0.0;
	GrenadeReloadingTime = 3.0;

	CentereExplosion = { 0.0,0.0,0.0 };
	FlagIsShoot = false;
	CurrentAnimationRadius = 0.0;
	DamageKoef = 0.0;	//must be in range [0.0:1.0]
}

double UGrenadeShootComponent::GetGrenadeMaxTime() const noexcept{
	return GreandeMaxTime;
}

void UGrenadeShootComponent::StartShoot(double UserDamageKoef, FVector PlayerStartShootPos) {
	GrenadesCountUsed += 1;
	CentereExplosion = PlayerStartShootPos;
	DamageKoef = UserDamageKoef;
	FlagIsShoot = true;
}

void UGrenadeShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGrenadeShootComponent::StartReload() {
	ReloadingFlag = true;
}

void UGrenadeShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ReloadingFlag) {
		CurrentReloadingTime += DeltaTime;

		if (CurrentReloadingTime >= GrenadeReloadingTime) {
			CurrentReloadingTime = 0.0;
			GrenadesCountUsed = 0;
			ReloadingFlag = false;

			//	Broadcast about reload end
			if (IsReload.IsBound()) {
				IsReload.Broadcast();
			}

		}
		return;
	}

	if (FlagIsShoot && CurrentAnimationRadius <= MaxDamageLen && GrenadesCountUsed <= GrenadesCount) {

		CurrentAnimationRadius += 15;
		DrawDebugSphere(GetWorld(), CentereExplosion, CurrentAnimationRadius, 40, FColor::Red, false, 0.2);

		if (CurrentAnimationRadius > MaxDamageLen) {

			TArray<FHitResult> Hits;
			FCollisionQueryParams CollisionParams;
			//CollisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			CollisionParams.AddIgnoredActor(GetOwner());

			bool FlagFoundCollisions = GetWorld()->SweepMultiByChannel(Hits, CentereExplosion, CentereExplosion,
				FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(MaxDamageLen), CollisionParams);

			if (FlagFoundCollisions) {
				for (const FHitResult& HitResult : Hits)
				{
					AActor* HitActor = HitResult.GetActor();
					if (HitActor && HitActor->IsA(ABoxActor::StaticClass())) {
						ABoxActor* BoxTmp = Cast<ABoxActor>(HitResult.GetActor());
						BoxTmp->GetDamage(Damage * DamageKoef);
					}
					else if (HitActor && HitActor->IsA(AEnemyActorBasic::StaticClass())) {
						AEnemyActorBasic* EnemyTmp = Cast<AEnemyActorBasic>(HitResult.GetActor());
						EnemyTmp->GetDamage(Damage * DamageKoef);
					}
				}
			}

			//	Nulling all params and let them to be valid
			CentereExplosion = { 0.0,0.0,0.0 };
			FlagIsShoot = false;
			CurrentAnimationRadius = 0.0;
			DamageKoef = 0.0;
		}
	}
	else {
		FlagIsShoot = false;
	}
}