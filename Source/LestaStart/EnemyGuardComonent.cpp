#include "EnemyGuardComonent.h"
#include "Net/UnrealNetwork.h"

UEnemyGuardComonent::UEnemyGuardComonent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

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

void UEnemyGuardComonent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyGuardComonent, EnemyDirection);
	DOREPLIFETIME(UEnemyGuardComonent, AccuracyOfPointMoving);
	DOREPLIFETIME(UEnemyGuardComonent, FlagOfBasicDirection);
	DOREPLIFETIME(UEnemyGuardComonent, IsThisEnemyGuard);
	DOREPLIFETIME(UEnemyGuardComonent, IsUserVisible);
	DOREPLIFETIME(UEnemyGuardComonent, VisibilityOfPlayer);
	DOREPLIFETIME(UEnemyGuardComonent, LengthOfWalking);
	DOREPLIFETIME(UEnemyGuardComonent, SpeedOfRunEnemy);
	DOREPLIFETIME(UEnemyGuardComonent, SpeedOfWalkEnemy);
	DOREPLIFETIME(UEnemyGuardComonent, NewPosDelegate);
}

void UEnemyGuardComonent::InitialSetup_Implementation(const FVector& Position) {
	CurrentPos = Position;
	InitialPos = Position;
}

void UEnemyGuardComonent::GetHitRes_Implementation(FHitResult HitRes) {

	Hit = HitRes;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && Hit.GetActor() == PlayerController->GetPawn() && Hit.Distance <= VisibilityOfPlayer)
		{
			IsUserVisible = true;
			return;
		}
	}
	
	IsUserVisible = false;
	
}

void UEnemyGuardComonent::BeginPlay()
{
	Super::BeginPlay();
}


void UEnemyGuardComonent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())return;

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

		if (!Hit.GetActor())return;

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