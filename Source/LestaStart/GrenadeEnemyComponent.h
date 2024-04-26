//	Implements the ability of the enemy to explode and cause damage

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GrenadeEnemyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UGrenadeEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrenadeEnemyComponent();

	UFUNCTION()
		void SetExplosionRadius(const double & Radius);

	UFUNCTION()
		void StartExplosion();

	UPROPERTY(EditAnywhere)
		double ExplosionRadius;

	UPROPERTY(EditAnywhere)
		double Damage;

	UPROPERTY(EditAnywhere)
		bool FlagIsGrenade;

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	//	To check, was collision with actor
	TSet<AActor*> HitActors;
};
