#include "FollowingPlayerComponent.h"

UFollowingPlayerComponent::UFollowingPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IsFollowPlayerFlag = false;
	IsAlreadyExplosion = false;

	SpeedOfRunEnemy = 500.0;
	VisibilityOfPlayer = 1100.0;
	DistanceOfStartExplosion = 50.0;
}

void UFollowingPlayerComponent::GetTargetHit(FHitResult HitRes) {
	Hit = HitRes;
}

void UFollowingPlayerComponent::InitStartPosition(FVector3d NewPos) {
	CurrentPos = NewPos;
	TargetPos = NewPos;
}

void UFollowingPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFollowingPlayerComponent::SetFollowingPlayer(bool Flag) {
	IsFollowPlayerFlag = Flag;
}

void UFollowingPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsFollowPlayerFlag||IsAlreadyExplosion)return;

	auto User = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!Hit.IsValidBlockingHit() || Hit.Distance > VisibilityOfPlayer || Hit.GetActor() != User ) {
		return;
	}

	if (Hit.Distance <= DistanceOfStartExplosion&& Hit.GetActor() == User) {

		IsAlreadyExplosion = true;

		if (ExplosionStart.IsBound()) {
			ExplosionStart.Broadcast();
		}
		return;
	}

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