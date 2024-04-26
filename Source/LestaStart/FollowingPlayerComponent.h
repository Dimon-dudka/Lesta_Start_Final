//Provides an opportunity to constantly pursue the player

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FollowingPlayerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UFollowingPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFollowingPlayerComponent();

	//	Send Start Explosion request
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartOfExplosion);
	FStartOfExplosion ExplosionStart;

	//	Returns new calculated position
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnNewPosition, FVector, NewPos);
	FReturnNewPosition NewPosDelegate;

	UPROPERTY(EditAnywhere)
		double SpeedOfRunEnemy;

	UPROPERTY(EditAnywhere)
		double VisibilityOfPlayer;

	UPROPERTY(EditAnywhere)
		double DistanceOfStartExplosion;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	bool IsFollowPlayerFlag,IsAlreadyExplosion;

	FHitResult Hit;

	FVector3d  CurrentPos, TargetPos;	

	FVector2D CalculationPos;

public:	

	UFUNCTION()
		void SetFollowingPlayer(bool Flag);

	UFUNCTION()
		void GetTargetHit(FHitResult HitRes);

	UFUNCTION()
		void InitStartPosition(FVector3d NewPos);
};