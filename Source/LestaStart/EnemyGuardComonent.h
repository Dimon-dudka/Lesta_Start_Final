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
	UPROPERTY(Replicated)
	FReturnNewPosition NewPosDelegate;

	//	Guard - means that enemy gonna walk 
	UPROPERTY(EditAnywhere,Replicated)
		bool IsThisEnemyGuard;

	UPROPERTY(EditAnywhere, Replicated)
		double LengthOfWalking;

	UPROPERTY(EditAnywhere, Replicated)
		double SpeedOfRunEnemy;

	UPROPERTY(EditAnywhere, Replicated)
		double SpeedOfWalkEnemy;

	UPROPERTY(EditAnywhere, Replicated)
		double VisibilityOfPlayer;

	UPROPERTY(EditAnywhere, Replicated)
		double AccuracyOfPointMoving;

	UPROPERTY(EditAnywhere, Replicated)
		bool EnemyDirection;	// 0 - X, 1 - Y

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	bool IsUserVisible;

	UPROPERTY(Replicated)
	bool FlagOfBasicDirection;

	UPROPERTY(Replicated)
	FHitResult Hit;

	UPROPERTY(Replicated)
	TArray<FVector3d> WayToReturn;

	UPROPERTY(Replicated)
	FVector3d  CurrentPos;

	UPROPERTY(Replicated)
	FVector3d InitialPos;

	UPROPERTY(Replicated)
	FVector3d TargetPos;

	//	For calculating vector on each step to target point
	UPROPERTY(Replicated)
	FVector2D CalculationPos;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
		void InitialSetup(const FVector& Position);

	UFUNCTION(Server,Unreliable)
		void GetHitRes(FHitResult HitRes);
};