#include "SpawnPoint.h"

USpawnPoint::USpawnPoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();	
}


void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}