#include "TraceComponent.h"

UTraceComponent::UTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Pawn) {
		FHitResult Hit;
		FVector TraceStart = GetOwner()->GetActorLocation();
		FVector TraceEnd = Pawn->GetActorLocation();
		bool bBlockHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn);

		if (Hit.bBlockingHit&&TraceResult.IsBound()) {
			TraceResult.Broadcast(Hit);
		}
	}
}