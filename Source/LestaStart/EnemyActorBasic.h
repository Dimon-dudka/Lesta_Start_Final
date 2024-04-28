//	Represents the basic enemy charachter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ActorInterface.h"

#include "Components/BoxComponent.h"

#include "HealthComponent.h"
#include "TraceComponent.h"
#include "ShootLaserComponent.h"
#include "DestroyAnimationComponent.h"
#include "HPPrintComponent.h"
#include "EnemyGuardComonent.h"
#include "GrenadeShootComponent.h"
#include "FollowingPlayerComponent.h"

#include "EnemyActorBasic.generated.h"

UCLASS()
class LESTASTART_API AEnemyActorBasic : public AActor , public IActorInterface
{
	GENERATED_BODY()
	
public:	
	AEnemyActorBasic();

	UPROPERTY(EditAnywhere)
		TObjectPtr<UFollowingPlayerComponent> FollowingComp;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UGrenadeShootComponent> GrenadeComponent;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UEnemyGuardComonent> GuardComp;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UHPPrintComponent> PrintHP;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDestroyAnimationComponent> DestroyComp;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UShootLaserComponent> LazerShoot;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UTraceComponent> Tracer;

protected:

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION()
		void GetDestroyed();

	UFUNCTION()
		void GetNewLocation(FVector NewLocation);

public:	

	UFUNCTION()
		void GetNullHPInfo();

	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;
};