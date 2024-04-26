//	Provides user ability to shoot lazer to center view point

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "BoxActor.h"
#include "EnemyActorBasic.h"

#include "LazerShootUserComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API ULazerShootUserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULazerShootUserComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfReloading);
	FEndOfReloading IsReload;

	UPROPERTY(EditAnywhere)
		double MaxShootingTime;

	UPROPERTY(EditAnywhere)
		double ReloadingTime;

	UPROPERTY(EditAnywhere)
		double Damage;

	UPROPERTY(EditAnywhere)
		double MaxLength;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	bool ShootingFlag,IsReloading;

	double CurrentShootingTime, CurrentReloadingTime;

public:	

	UFUNCTION()
		void GetShooting(const bool& Shooting);

	UFUNCTION()
		void StartReload();
};