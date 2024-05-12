//	Spawns an actor

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API USpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpawnerComponent();

	UPROPERTY(EditAnywhere,Replicated)
		TObjectPtr<UClass> SpawnActorType {nullptr};

	DECLARE_DYNAMIC_DELEGATE_RetVal(FTransform, FGetSpawnTransform);
	UPROPERTY(Replicated)
		FGetSpawnTransform GetSpawnTransform;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr <AActor> SpawnActor;

	UFUNCTION(Server,Unreliable)
		void SpawningActor();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void BeginPlay() override;
};