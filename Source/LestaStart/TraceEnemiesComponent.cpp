#include "TraceEnemiesComponent.h"
#include "Net/UnrealNetwork.h"

UTraceEnemiesComponent::UTraceEnemiesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UTraceEnemiesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTraceEnemiesComponent, TraceResult);
}

void UTraceEnemiesComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UTraceEnemiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())return;

	bool bBlockHit;

	APawn* FinalPawn{ nullptr };
	double MinDistance{(double)INT_MAX};

	FHitResult Hit;
	FVector TraceStart{ GetOwner()->GetActorLocation() }, TraceEnd;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
		if (!It->Get())continue;
		if (!It->Get()->GetPawn())continue;

		TraceEnd = It->Get()->GetPawn()->GetActorLocation();
		bBlockHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn);

		if (Hit.GetActor() != It->Get()->GetPawn())continue;

		if (Hit.Distance < MinDistance) {
			MinDistance = Hit.Distance;
			FinalPawn = It->Get()->GetPawn();
		}
	}

	if (FinalPawn) {

		TraceEnd = FinalPawn->GetActorLocation();
		
		bBlockHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn);

		if (!TraceResult.IsBound())return;

		if (Hit.bBlockingHit && FinalPawn ==Hit.GetActor() ) {
			TraceResult.Broadcast(Hit);
		}
		else if (Hit.bBlockingHit && FinalPawn != Hit.GetActor()) {
			TraceResult.Broadcast({});
		}
	}
	else {
		if (!TraceResult.IsBound())return;

		TraceResult.Broadcast({});
	}
}