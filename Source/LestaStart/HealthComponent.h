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

	UPROPERTY(EditAnywhere, Replicated)
		double Health;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetHPValue, double, HP);
	UPROPERTY(Replicated)
		FGetHPValue GetHPValue;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGetNullHPDelegate);
	UPROPERTY(Replicated)
		FGetNullHPDelegate GetNullHP;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

public:	
	//	Become damage API
	UFUNCTION(Server, Unreliable)
		void BecomeDamage(const double& BecomeHPDamage);

	//	Return HP for actor print
	UFUNCTION()
		double GetHP()const;

	UFUNCTION(Server, Unreliable)
		void SetHP(const double& HP);	
};