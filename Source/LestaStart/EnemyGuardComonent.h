//	Provides an opportunity for the enemy to guard the area,
//	attack the player, chase him and return to the place of protection
//  in case of loss of sight of the player

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyGuardComonent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UEnemyGuardComonent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyGuardComonent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnNewPosition, FVector,NewPos);
	FReturnNewPosition NewPosDelegate;

	//	Guard - means that enemy gonna walk 
	UPROPERTY(EditAnywhere)
		bool IsThisEnemyGuard;

	UPROPERTY(EditAnywhere)
		double LengthOfWalking;

	UPROPERTY(EditAnywhere)
		double SpeedOfRunEnemy;

	UPROPERTY(EditAnywhere)
		double SpeedOfWalkEnemy;

	UPROPERTY(EditAnywhere)
		double VisibilityOfPlayer;

	UPROPERTY(EditAnywhere)
		double AccuracyOfPointMoving;

	UPROPERTY(EditAnywhere)
		bool EnemyDirection;	// 0 - X, 1 - Y

protected:
	virtual void BeginPlay() override;

	bool FlagOfBasicDirection,IsUserVisible;

	FHitResult Hit;

	TArray<FVector3d> WayToReturn;

	FVector3d  CurrentPos, InitialPos, TargetPos;

	//	For calculating vector on each step to target point
	FVector2D CalculationPos;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void InitialSetup(const FVector& Position);

	UFUNCTION()
		void GetHitRes(FHitResult HitRes);
};
