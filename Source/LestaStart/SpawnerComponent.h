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

	UPROPERTY(EditAnywhere)
		UClass* SpawnActorType {nullptr};

	DECLARE_DYNAMIC_DELEGATE_RetVal(FTransform, FGetSpawnTransform);
	UPROPERTY()
		FGetSpawnTransform GetSpawnTransform;

	UPROPERTY(EditAnywhere)
		AActor* SpawnActor;

	UFUNCTION()
		void SpawningActor();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;
};