//	Represents the basic enemy charachter

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorInterface.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "TraceEnemiesComponent.h"
#include "LazerShootComponent.h"
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

	//	To change the HP label above the actor.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHPUIEnemy, double, HP);
	FChangeHPUIEnemy ChangeHPDelegate;

	//	To shoot with lazer
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootStatus, bool, Flag);
	UPROPERTY(Replicated)
		FShootStatus ShootStatus;

	//	For the bomber enemy
	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UFollowingPlayerComponent> FollowingComp;

	//	For the bomber enemy to explose
	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UGrenadeShootComponent> GrenadeComponent;

	//	For the guardian enemy
	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UEnemyGuardComonent> GuardComp;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UHPPrintComponent> PrintHP;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UDestroyAnimationComponent> DestroyComp;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<ULazerShootComponent> LazerShootComp;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UTraceEnemiesComponent> TraceComp;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//	Destroy actor after all necessaryes procedures
	UFUNCTION(Server, Unreliable)
		void GetDestroyed();

	//	For update HP text above actor
	UFUNCTION(NetMulticast, Unreliable)
		void ChangeHP(double HP);

public:	

	UFUNCTION(Server, Unreliable)
		void GetNewLocation(FVector NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
		void GetNullHPInfo();

	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;
};