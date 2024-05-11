#include "FollowingPlayerComponent.h"
#include "Net/UnrealNetwork.h"

UFollowingPlayerComponent::UFollowingPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	IsFollowPlayerFlag = false;
	IsAlreadyExplosion = false;

	SpeedOfRunEnemy = 500.0;
	VisibilityOfPlayer = 1100.0;
	DistanceOfStartExplosion = 50.0;
}

void UFollowingPlayerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFollowingPlayerComponent, ExplosionStart);
	DOREPLIFETIME(UFollowingPlayerComponent, NewPosDelegate);
	DOREPLIFETIME(UFollowingPlayerComponent, SpeedOfRunEnemy);
	DOREPLIFETIME(UFollowingPlayerComponent, VisibilityOfPlayer);
	DOREPLIFETIME(UFollowingPlayerComponent, DistanceOfStartExplosion);
	DOREPLIFETIME(UFollowingPlayerComponent, IsFollowPlayerFlag);
	DOREPLIFETIME(UFollowingPlayerComponent, IsAlreadyExplosion);
	DOREPLIFETIME(UFollowingPlayerComponent, Hit);
	DOREPLIFETIME(UFollowingPlayerComponent, CurrentPos);
	DOREPLIFETIME(UFollowingPlayerComponent, TargetPos);
	DOREPLIFETIME(UFollowingPlayerComponent, CalculationPos);
}

void UFollowingPlayerComponent::GetTargetHit_Implementation(FHitResult HitRes) {
	Hit = HitRes;
}

void UFollowingPlayerComponent::InitStartPosition_Implementation(FVector3d NewPos) {
	CurrentPos = NewPos;
	TargetPos = NewPos;
}

void UFollowingPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFollowingPlayerComponent::SetFollowingPlayer_Implementation(bool Flag) {
	IsFollowPlayerFlag = Flag;
}

void UFollowingPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())return;

	if (!IsFollowPlayerFlag||IsAlreadyExplosion)return;

	if (!Hit.IsValidBlockingHit() || Hit.Distance > VisibilityOfPlayer) return;

	if (Hit.Distance <= DistanceOfStartExplosion) {

		IsAlreadyExplosion = true;

		if (ExplosionStart.IsBound()) {
			ExplosionStart.Broadcast();
		}
		return;
	}

	if (!Hit.GetActor())return;

	TargetPos = Hit.GetActor()->GetActorLocation();

	CalculationPos = { TargetPos.X - CurrentPos.X,TargetPos.Y - CurrentPos.Y };
	CalculationPos.Normalize();

	CalculationPos *= SpeedOfRunEnemy*DeltaTime;

	CurrentPos.X += CalculationPos.X;
	CurrentPos.Y += CalculationPos.Y;

	if (NewPosDelegate.IsBound()) {
		NewPosDelegate.Broadcast(CurrentPos);
	}
}