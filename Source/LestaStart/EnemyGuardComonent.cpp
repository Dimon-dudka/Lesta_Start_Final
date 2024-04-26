#include "EnemyGuardComonent.h"

UEnemyGuardComonent::UEnemyGuardComonent()
{
	PrimaryComponentTick.bCanEverTick = true;

	EnemyDirection = 0;

	AccuracyOfPointMoving = 2.0;

	FlagOfBasicDirection = false;
	IsThisEnemyGuard = false;
	IsUserVisible = false;

	VisibilityOfPlayer = 500.0;
	LengthOfWalking = 300.0;
	SpeedOfRunEnemy = 300.0;
	SpeedOfWalkEnemy = 150.0;
}

void UEnemyGuardComonent::InitialSetup(const FVector& Position) {
	CurrentPos = Position;
	InitialPos = Position;
}

void UEnemyGuardComonent::GetHitRes(FHitResult HitRes) {

	Hit = HitRes;
	
	if (Hit.GetActor() == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)&&Hit.Distance<= VisibilityOfPlayer) {
		IsUserVisible = true;
	}
	else {
		IsUserVisible = false;
	}
}

void UEnemyGuardComonent::BeginPlay()
{
	Super::BeginPlay();
}


void UEnemyGuardComonent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsThisEnemyGuard)return;

	//	Already in start position
	if (!IsUserVisible && WayToReturn.IsEmpty()) {	
		
		if (EnemyDirection == 0) {

			FlagOfBasicDirection ? CurrentPos.X += SpeedOfWalkEnemy * DeltaTime 
				: CurrentPos.X -= SpeedOfWalkEnemy * DeltaTime;

			if(CurrentPos.X>=InitialPos.X+LengthOfWalking){
				FlagOfBasicDirection = !FlagOfBasicDirection;
			}
			else if (CurrentPos.X <= InitialPos.X - LengthOfWalking) {
				FlagOfBasicDirection = !FlagOfBasicDirection;
			}

		}
		else {

			FlagOfBasicDirection ? CurrentPos.Y += SpeedOfWalkEnemy * DeltaTime 
				: CurrentPos.Y -= SpeedOfWalkEnemy * DeltaTime;

			if (CurrentPos.Y >= InitialPos.Y + LengthOfWalking) {
				FlagOfBasicDirection = !FlagOfBasicDirection;
			}
			else if (CurrentPos.Y <= InitialPos.Y - LengthOfWalking) {
				FlagOfBasicDirection = !FlagOfBasicDirection;
			}
		}

		TargetPos = CurrentPos;
	}
	//	Has to return to the start point
	else if (!IsUserVisible && !WayToReturn.IsEmpty()) {	

		if (FMath::IsNearlyEqual(TargetPos.X, CurrentPos.X, AccuracyOfPointMoving) 
			|| FMath::IsNearlyEqual(TargetPos.Y, CurrentPos.Y, AccuracyOfPointMoving)) {

			TargetPos = WayToReturn[WayToReturn.Num() - 1];
			WayToReturn.RemoveAt(WayToReturn.Num() - 1);

			CalculationPos = { TargetPos.X - CurrentPos.X,TargetPos.Y - CurrentPos.Y };
			CalculationPos.Normalize();

			CalculationPos *= SpeedOfRunEnemy * DeltaTime;
		}

		CurrentPos.X += CalculationPos.X;
		CurrentPos.Y += CalculationPos.Y;
	}
	//	User is visible and enemy is moving towards
	else {

		if (WayToReturn.IsEmpty()) {
			WayToReturn.Push(InitialPos);
		}

		if (FMath::IsNearlyEqual(TargetPos.X,CurrentPos.X, AccuracyOfPointMoving)
			|| FMath::IsNearlyEqual(TargetPos.Y, CurrentPos.Y, AccuracyOfPointMoving)) {
			WayToReturn.Push(CurrentPos);
			TargetPos = Hit.GetActor()->GetActorLocation();

			CalculationPos = { TargetPos.X - CurrentPos.X,TargetPos.Y - CurrentPos.Y };
			CalculationPos.Normalize();

			CalculationPos *= SpeedOfRunEnemy * DeltaTime;
		}

		CurrentPos.X += CalculationPos.X;
		CurrentPos.Y += CalculationPos.Y;
	}

	if (NewPosDelegate.IsBound()) {
		NewPosDelegate.Broadcast(CurrentPos);
	}
}