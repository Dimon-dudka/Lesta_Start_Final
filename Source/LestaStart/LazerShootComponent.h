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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfReloading);
	FEndOfReloading IsReload;

	UPROPERTY(EditAnywhere)
		bool MustShoot;

	UPROPERTY(EditAnywhere)
		bool IsPlayer;

	UPROPERTY(EditAnywhere)
		double MaxShootingTime;

	UPROPERTY(EditAnywhere)
		double ReloadingTime;

	UPROPERTY(EditAnywhere)
		double Damage;

	UPROPERTY(EditAnywhere)
		double MaxLength;

protected:
	virtual void BeginPlay() override;

	double CurrentShootingTime, CurrentReloadingTime;

	FHitResult Hit;

	bool ShootingFlag, IsReloading;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void GetHit(FHitResult HitRes);

	UFUNCTION()
		void StartReload();

	UFUNCTION()
		void ShootStatus(bool Flag);
};