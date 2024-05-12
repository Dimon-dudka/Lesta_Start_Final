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

	//	Become damage API
	UFUNCTION(Server, Unreliable)
		void BecomeDamage(const double& BecomeHPDamage);

	//	Return HP for actor print
	UFUNCTION()
		double GetHP()const;

	//	Initial HP setup
	UFUNCTION(Server, Unreliable)
		void SetHP(const double& HP);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetHPValue, double, HP);
	UPROPERTY(Replicated)
		FGetHPValue GetHPValue;

	//	Info about actors death
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetNullHPDelegate);
	UPROPERTY(Replicated)
		FGetNullHPDelegate GetNullHP;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

private:

	UPROPERTY(EditAnywhere, Replicated)
		double Health;
};