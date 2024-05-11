//	Lazer component
//	Its use FHitResults to shoot

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorInterface.h"
#include "LazerShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API ULazerShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULazerShootComponent();

	//	Info about end of reloading
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfReloading);
	UPROPERTY(Replicated)
		FEndOfReloading IsReload;

	//	The setting is about whether the actor should shoot
	UPROPERTY(EditAnywhere, Replicated)
		bool MustShoot;

	//	Flag is owner player or not
	UPROPERTY(EditAnywhere, Replicated)
		bool IsPlayer;

	UPROPERTY(EditAnywhere, Replicated)
		double MaxShootingTime;

	UPROPERTY(EditAnywhere, Replicated)
		double ReloadingTime;

	UPROPERTY(EditAnywhere, Replicated)
		double Damage;

	//	Max damage length
	UPROPERTY(EditAnywhere, Replicated)
		double MaxLength;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	UPROPERTY(Replicated)
		double CurrentShootingTime;

	UPROPERTY(Replicated)
		double CurrentReloadingTime;

	UPROPERTY(Replicated)
		FHitResult Hit;

	UPROPERTY(Replicated)
		bool ShootingFlag;

	UPROPERTY(Replicated)
		bool IsReloading;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
		void GetHit(FHitResult HitRes);

	UFUNCTION(Server, Unreliable)
		void StartReload();

	UFUNCTION(Server, Unreliable)
		void ShootStatus(bool Flag);
};