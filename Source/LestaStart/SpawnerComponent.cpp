#include "SpawnerComponent.h"
#include "Net/UnrealNetwork.h"

USpawnerComponent::USpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USpawnerComponent::SpawningActor_Implementation() {

	if (!GetOwner()->HasAuthority())return;

	if (GetSpawnTransform.IsBound() && SpawnActorType) {

		FTransform Transform{ GetSpawnTransform.Execute() };
		UWorld* World{ GetWorld() };
		World->SpawnActor<AActor>(SpawnActorType, Transform);
	}
}

void USpawnerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USpawnerComponent, SpawnActorType);
	DOREPLIFETIME(USpawnerComponent, GetSpawnTransform);
	DOREPLIFETIME(USpawnerComponent, SpawnActor);
}

void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}