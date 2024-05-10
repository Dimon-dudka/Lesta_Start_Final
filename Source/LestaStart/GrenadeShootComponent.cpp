#include "GrenadeShootComponent.h"
#include "Net/UnrealNetwork.h"

UGrenadeShootComponent::UGrenadeShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//	In user charachter needs to set this flag in true
	//	In enemy - depends of enemy type
	IsGrenade = true;

	SetIsReplicated(true);

	Damage = 50.0;
	MaxDamageLen = 700.0;
	GreandeMaxTime = 1.0;

	GrenadesCountUsed = 0;
	GrenadesCount = 3;
	ReloadingFlag = false;
	CurrentReloadingTime = 0.0;
	GrenadeReloadingTime = 3.0;
	AnimationSpeed = 1000;

	CentereExplosion = { 0.0,0.0,0.0 };
	FlagIsShoot = false;
	CurrentAnimationRadius = 0.0;
	DamageKoef = 0.0;	//must be in range [0.0:1.0]
}

void UGrenadeShootComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGrenadeShootComponent, IsGrenade);
	DOREPLIFETIME(UGrenadeShootComponent, Damage);
	DOREPLIFETIME(UGrenadeShootComponent, MaxDamageLen);
	DOREPLIFETIME(UGrenadeShootComponent, GreandeMaxTime);
	DOREPLIFETIME(UGrenadeShootComponent, GrenadesCountUsed);
	DOREPLIFETIME(UGrenadeShootComponent, GrenadesCount);
	DOREPLIFETIME(UGrenadeShootComponent, ReloadingFlag);
	DOREPLIFETIME(UGrenadeShootComponent, CurrentReloadingTime);
	DOREPLIFETIME(UGrenadeShootComponent, GrenadeReloadingTime);
	DOREPLIFETIME(UGrenadeShootComponent, CentereExplosion);
	DOREPLIFETIME(UGrenadeShootComponent, FlagIsShoot);
	DOREPLIFETIME(UGrenadeShootComponent, DamageKoef);
	DOREPLIFETIME(UGrenadeShootComponent, EndOfExpl);
}

double UGrenadeShootComponent::GetGrenadeMaxTime() const noexcept{
	return GreandeMaxTime;
}

void UGrenadeShootComponent::StartShoot_Implementation(double UserDamageKoef, FVector PlayerStartShootPos) {
	GrenadesCountUsed += 1;
	CentereExplosion = PlayerStartShootPos;
	DamageKoef = UserDamageKoef;
	FlagIsShoot = true;
}

void UGrenadeShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGrenadeShootComponent::StartReload_Implementation() {
	ReloadingFlag = true;
}

void UGrenadeShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsGrenade)return;

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

		CurrentAnimationRadius += AnimationSpeed*DeltaTime;
		DrawDebugSphere(GetWorld(), CentereExplosion, CurrentAnimationRadius, 40, FColor::Red, false, 0.2);

		if (CurrentAnimationRadius > MaxDamageLen) {

			TArray<FHitResult> Hits;

			TSet<AActor*> HitActors;

			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(GetOwner());

			bool FlagFoundCollisions = GetWorld()->SweepMultiByChannel(Hits, CentereExplosion, CentereExplosion,
				FQuat::Identity, ECC_PhysicsBody, FCollisionShape::MakeSphere(MaxDamageLen), CollisionParams);

			if (FlagFoundCollisions&& GetOwner()->HasAuthority()) {
				for (const FHitResult& HitResult : Hits)
				{
					AActor* HitActor = HitResult.GetActor();

					if (HitActors.Contains(HitActor))continue;

					HitActors.Add(HitActor);

					IActorInterface* ActorInterfaceTmp = Cast<IActorInterface>(HitResult.GetActor());

					if (!ActorInterfaceTmp)continue;

					ActorInterfaceTmp->GetDamage(Damage * DamageKoef);
				}
			}
			//	Nulling all params and let them to be valid
			CentereExplosion = { 0.0,0.0,0.0 };
			FlagIsShoot = false;
			CurrentAnimationRadius = 0.0;
			DamageKoef = 0.0;

			if (EndOfExpl.IsBound()) {
				EndOfExpl.Broadcast();
			}
		}
	}
	else {
		FlagIsShoot = false;
	}
}