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
	FEndOfReloading IsReload;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfExplosion);
	FEndOfExplosion EndOfExpl;

	UPROPERTY(EditAnywhere)
		int32 GrenadesCount;

	UPROPERTY(EditAnywhere)
		double GrenadeReloadingTime;

	UPROPERTY(EditAnywhere)
		bool IsGrenade;

	//	Time of pressing button to maximize damage
	UPROPERTY(EditAnywhere)
		double GreandeMaxTime;

	UPROPERTY(EditAnywhere)
		int32 AnimationSpeed;

	UPROPERTY(EditAnywhere)
		double Damage;

	UPROPERTY(EditAnywhere)
		double MaxDamageLen;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	FVector CentereExplosion;

	bool FlagIsShoot,ReloadingFlag;
	
	double CurrentAnimationRadius, DamageKoef, CurrentReloadingTime;

	int32 GrenadesCountUsed;

public:	

	UFUNCTION()
		void StartShoot(double UserDamageKoef,FVector PlayerStartShootPos);

	UFUNCTION()
		void StartReload();

	//	Returns time of pressing button to maximize damage
	UFUNCTION()
		double GetGrenadeMaxTime()const noexcept;
};