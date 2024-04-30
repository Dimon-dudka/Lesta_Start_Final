#include "SpawnerComponent.h"

USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpawnerComponent::SpawningActor() {
	if (GetSpawnTransform.IsBound() && SpawnActorType) {

		FTransform Transform{ GetSpawnTransform.Execute() };
		UWorld* World{ GetWorld() };
		World->SpawnActor<AActor>(SpawnActorType, Transform);
	}
}

void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}