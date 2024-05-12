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

	UFUNCTION(Server, Unreliable)
		void StartShoot(double UserDamageKoef, FVector PlayerStartShootPos);

	UFUNCTION(Server, Unreliable)
		void StartReload();

	//	Returns time of pressing button to maximize damage
	UFUNCTION()
		double GetGrenadeMaxTime()const noexcept;

	//	Return answer about end of reloading
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfReloading);
	UPROPERTY(Replicated)
		FEndOfReloading IsReload;

	//	Info about end of explosion
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfExplosion);
	UPROPERTY(Replicated)
		FEndOfExplosion EndOfExpl;

	//	For enemy settings
	UPROPERTY(EditAnywhere, Replicated)
		bool IsGrenade;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void BeginPlay() override;

private:

	UPROPERTY(Replicated)
		FVector CentereExplosion;

	//	Flag about is explosion going
	UPROPERTY(Replicated)
		bool FlagIsShoot;

	UPROPERTY(Replicated)
		bool ReloadingFlag;

	//	For players grenade in range [0:1]
	UPROPERTY(Replicated)
		double DamageKoef;

	UPROPERTY(Replicated)
		double CurrentReloadingTime;

	UPROPERTY(Replicated)
		int32 GrenadesCountUsed;

	UPROPERTY(EditAnywhere, Replicated)
		int32 GrenadesCount;

	UPROPERTY(EditAnywhere, Replicated)
		double GrenadeReloadingTime;

	//	Time of pressing button to maximize damage
	UPROPERTY(EditAnywhere, Replicated)
		double GreandeMaxTime;

	UPROPERTY(EditAnywhere)
		int32 AnimationSpeed;

	UPROPERTY(EditAnywhere, Replicated)
		double Damage;

	//	Max radius where every actor become damage
	UPROPERTY(EditAnywhere, Replicated)
		double MaxDamageLen;

	UPROPERTY()
		double CurrentAnimationRadius;
};