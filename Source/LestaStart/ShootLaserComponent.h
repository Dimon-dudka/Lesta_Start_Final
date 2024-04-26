//	Lazer Shoot component of enemy

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BoxActor.h"

#include "ShootLaserComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UShootLaserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootLaserComponent();

	UPROPERTY(EditAnywhere)
		bool IsLazerWeapon;

	UPROPERTY(EditAnywhere)
		double Damage;

	UPROPERTY(EditAnywhere)
		double MaxDamageLen;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	FHitResult HitRes;
	bool ShootingFlag;

public:	

	UFUNCTION()
		void Shoot(FHitResult Hit);
};