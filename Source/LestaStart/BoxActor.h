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

	UPROPERTY(EditAnywhere,Replicated)
		TObjectPtr<UHealthComponent> Health;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
		double DestroyRadius;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	//UFUNCTION(Server, Unreliable)
	UFUNCTION()
		virtual void GetDamage(const double& Damage) override;

	UFUNCTION(Server, Unreliable)
		void GetNullHPInfo();

	UFUNCTION(Server, Unreliable)
		void GetDestroyed();
};