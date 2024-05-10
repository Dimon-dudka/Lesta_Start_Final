//	Provides an interface user to shoot grenade

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ActorInterface.h"
#include "GrenadeShootComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UGrenadeShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrenadeShootComponent();

	//	Return answer about end of reloading
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfReloading);
	UPROPERTY(Replicated)
		FEndOfReloading IsReload;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfExplosion);
	UPROPERTY(Replicated)
		FEndOfExplosion EndOfExpl;

	UPROPERTY(EditAnywhere, Replicated)
		int32 GrenadesCount;

	UPROPERTY(EditAnywhere, Replicated)
		double GrenadeReloadingTime;

	UPROPERTY(EditAnywhere, Replicated)
		bool IsGrenade;

	//	Time of pressing button to maximize damage
	UPROPERTY(EditAnywhere, Replicated)
		double GreandeMaxTime;

	UPROPERTY(EditAnywhere)
		int32 AnimationSpeed;

	UPROPERTY(EditAnywhere, Replicated)
		double Damage;

	UPROPERTY(EditAnywhere, Replicated)
		double MaxDamageLen;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
		FVector CentereExplosion;

	UPROPERTY(Replicated)
		bool FlagIsShoot;

	UPROPERTY(Replicated)
		bool ReloadingFlag;
	
	double CurrentAnimationRadius;

	UPROPERTY(Replicated)
		double DamageKoef;

	UPROPERTY(Replicated)
		double CurrentReloadingTime;

	UPROPERTY(Replicated)
		int32 GrenadesCountUsed;

public:	

	UFUNCTION(Server,Unreliable)
		void StartShoot(double UserDamageKoef,FVector PlayerStartShootPos);

	UFUNCTION(Server, Unreliable)
		void StartReload();

	//	Returns time of pressing button to maximize damage
	UFUNCTION()
		double GetGrenadeMaxTime()const noexcept;
};