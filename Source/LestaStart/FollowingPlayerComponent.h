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
	UPROPERTY(Replicated)
	FStartOfExplosion ExplosionStart;

	//	Returns new calculated position
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnNewPosition, FVector, NewPos);
	UPROPERTY(Replicated)
	FReturnNewPosition NewPosDelegate;

	UPROPERTY(EditAnywhere, Replicated)
		double SpeedOfRunEnemy;

	UPROPERTY(EditAnywhere, Replicated)
		double VisibilityOfPlayer;

	UPROPERTY(EditAnywhere, Replicated)
		double DistanceOfStartExplosion;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	bool IsFollowPlayerFlag;

	UPROPERTY(Replicated)
	bool IsAlreadyExplosion;

	UPROPERTY(Replicated)
	FHitResult Hit;

	UPROPERTY(Replicated)
	FVector3d  CurrentPos;	

	UPROPERTY(Replicated)
	FVector3d TargetPos;

	UPROPERTY(Replicated)
	FVector2D CalculationPos;

public:	

	UFUNCTION(Server,Unreliable)
		void SetFollowingPlayer(bool Flag);

	UFUNCTION(Server, Unreliable)
		void GetTargetHit(FHitResult HitRes);

	UFUNCTION(Server, Unreliable)
		void InitStartPosition(FVector3d NewPos);
};