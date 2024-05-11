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

	//	Length of max security distance
	UPROPERTY(EditAnywhere, Replicated)
		double LengthOfWalking;

	UPROPERTY(EditAnywhere, Replicated)
		double SpeedOfRunEnemy;

	UPROPERTY(EditAnywhere, Replicated)
		double SpeedOfWalkEnemy;

	//	Max length of user visibility
	UPROPERTY(EditAnywhere, Replicated)
		double VisibilityOfPlayer;

	//	The accuracy of hitting the calculated point
	UPROPERTY(EditAnywhere, Replicated)
		double AccuracyOfPointMoving;

	//	In what global coordinate actor will be walking while security
	UPROPERTY(EditAnywhere, Replicated)
		bool EnemyDirection;	// 0 - X, 1 - Y

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//	Is some player visible
	UPROPERTY(Replicated)
		bool IsUserVisible;

	//	Àlag about the method of changing coordinates during protection (+ or -)
	UPROPERTY(Replicated)
		bool FlagOfBasicDirection;

	//	HitResult from trace component
	UPROPERTY(Replicated)
		FHitResult Hit;

	//	Way to initial point
	UPROPERTY(Replicated)
		TArray<FVector3d> WayToReturn;

	UPROPERTY(Replicated)
		FVector3d  CurrentPos;

	UPROPERTY(Replicated)
		FVector3d InitialPos;

	//	Point where was traced player
	UPROPERTY(Replicated)
		FVector3d TargetPos;

	//	For calculating vector on each step to target point (without Z-axis)
	UPROPERTY(Replicated)
		FVector2D CalculationPos;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
		void InitialSetup(const FVector& Position);

	UFUNCTION(Server,Unreliable)
		void GetHitRes(FHitResult HitRes);
};