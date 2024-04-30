//	Represents box that can be destroyed

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorInterface.h"
#include "HealthComponent.h"
#include "DestroyAnimationComponent.h"
#include "Components/BoxComponent.h"
#include "BoxActor.generated.h"

UCLASS()
class LESTASTART_API ABoxActor : public AActor, public IActorInterface
{
	GENERATED_BODY()
	
public:	
	ABoxActor();

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDestroyAnimationComponent> DestroyAnimation;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
		double DestroyRadius;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	

	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;

	UFUNCTION()
		void GetNullHPInfo();

	UFUNCTION()
		void GetDestroyed();
};