#include "GrenadeEnemyComponent.h"

#include "BoxActor.h"
#include "EnemyActorBasic.h"
#include "Core/LestaCharacter.h"

UGrenadeEnemyComponent::UGrenadeEnemyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	FlagIsGrenade = false;
	//IsAlreadyDamaged = false;

	ExplosionRadius = 100.0;
	Damage = 30.0;
}

void UGrenadeEnemyComponent::SetExplosionRadius(const double& Radius) {
	ExplosionRadius = Radius;
}

void UGrenadeEnemyComponent::StartExplosion()
{
	if (!FlagIsGrenade)return;

	TArray<FHitResult> Hits;

	bool FlagFoundCollisions = GetWorld()->SweepMultiByChannel(Hits, GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(), FQuat::Identity, ECC_PhysicsBody,
		FCollisionShape::MakeSphere(ExplosionRadius));

	if (FlagFoundCollisions ) {

		auto User = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		for (const FHitResult& HitResult : Hits)
		{
			AActor* HitActor = HitResult.GetActor();

			if (HitResult.Distance > ExplosionRadius || !HitActor|| HitActors.Contains(HitActor))continue;

			HitActors.Add(HitActor);

			if (HitResult.GetActor() == User) {
				ALestaCharacter* ActorTmp = Cast<ALestaCharacter>(HitResult.GetActor());
				ActorTmp->GetDamage(Damage);
			}
			else if (HitActor->IsA(ABoxActor::StaticClass())) {
				ABoxActor* BoxTmp = Cast<ABoxActor>(HitResult.GetActor());
				BoxTmp->GetDamage(Damage);
			}
			else if (HitActor->IsA(AEnemyActorBasic::StaticClass())) {
				AEnemyActorBasic* EnemyTmp = Cast<AEnemyActorBasic>(HitResult.GetActor());
				EnemyTmp->GetDamage(Damage);
			}
		}
	}
}


void UGrenadeEnemyComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UGrenadeEnemyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}