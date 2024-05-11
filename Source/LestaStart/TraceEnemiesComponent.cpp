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

	FHitResult Hit,FinalHit;
	FVector TraceStart{ GetOwner()->GetActorLocation() }, TraceEnd;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It) {
		if (!It->Get()||!It->Get()->GetPawn())continue;

		TraceEnd = It->Get()->GetPawn()->GetActorLocation();
		bBlockHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Pawn);

		//	If hited actor not player
		if (Hit.GetActor() != It->Get()->GetPawn())continue;

		//	Choise of closer player
		if (Hit.Distance < MinDistance) {
			MinDistance = Hit.Distance;
			FinalPawn = It->Get()->GetPawn();
			FinalHit = Hit;
		}
	}

	//	If at least one valid player is visible
	if (FinalPawn) {

		if (!TraceResult.IsBound())return;

		if (FinalHit.bBlockingHit && FinalPawn == FinalHit.GetActor() ) {
			TraceResult.Broadcast(FinalHit);
		}
		else if (FinalHit.bBlockingHit && FinalPawn != FinalHit.GetActor()) {
			TraceResult.Broadcast({});
		}
	}
	else if(TraceResult.IsBound()){
		TraceResult.Broadcast({});
	}
}