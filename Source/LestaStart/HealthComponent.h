//	Basic actor component to all actors with hp

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UPROPERTY(EditAnywhere)
		double Health;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetNullHPDelegate);
	FGetNullHPDelegate GetNullHP;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//	Become damage API
	UFUNCTION()
		void BecomeDamage(const double& BecomeHPDamage);

	//	Return HP for actor print
	UFUNCTION()
		double GetHP()const;

	UFUNCTION()
		void SetHP(const double& HP);	
};