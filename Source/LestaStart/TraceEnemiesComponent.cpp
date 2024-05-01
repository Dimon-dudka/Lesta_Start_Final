#include "TraceEnemiesComponent.h"

UTraceEnemiesComponent::UTraceEnemiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceEnemiesComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UTraceEnemiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Pawn) {
		FHitResult Hit;
		FVector TraceStart{ GetOwner()->GetActorLocation() }, TraceEnd{ Pawn->GetActorLocation() };
		bool bBlockHit{ GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn) };

		if (!TraceResult.IsBound())return;

		if (Hit.bBlockingHit && Pawn==Hit.GetActor() ) {
			TraceResult.Broadcast(Hit);
		}
		else if (Hit.bBlockingHit && Pawn != Hit.GetActor()) {
			TraceResult.Broadcast({});
		}
	}
}