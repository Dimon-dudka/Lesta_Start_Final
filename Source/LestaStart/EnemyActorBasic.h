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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeHPUIEnemy, double, HP);
	FChangeHPUIEnemy ChangeHPDelegate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootStatus, bool, Flag);
	UPROPERTY(Replicated)
	FShootStatus ShootStatus;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UFollowingPlayerComponent> FollowingComp;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UGrenadeShootComponent> GrenadeComponent;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UEnemyGuardComonent> GuardComp;

	//UPROPERTY(EditAnywhere)
	UPROPERTY(EditAnywhere)
		TObjectPtr<UHPPrintComponent> PrintHP;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UDestroyAnimationComponent> DestroyComp;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UBoxComponent> Box;

	//UPROPERTY(EditAnywhere, Replicated)
	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<ULazerShootComponent> LazerShootComp;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UTraceEnemiesComponent> TraceComp;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable)
		void GetDestroyed();

	UFUNCTION(NetMulticast, Unreliable)
		void ChangeHP(double HP);

public:	

	UFUNCTION(Server, Unreliable)
		void GetNewLocation(FVector NewLocation);

	UFUNCTION(NetMulticast, Unreliable)
		void GetNullHPInfo();

	//UFUNCTION(Server, Unreliable)
	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;
};