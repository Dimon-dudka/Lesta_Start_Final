#include "TracePlayersComponent.h"

UTracePlayersComponent::UTracePlayersComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IsTrace = false;

    MaxLengthOfTrace = 100000.0;
}

void UTracePlayersComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTracePlayersComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        FVector ViewLocation;
        FRotator ViewRotation;
        PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

        FVector End{ ViewLocation + (ViewRotation.Vector() * MaxLengthOfTrace) };

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("TraceFromPlayerView")), true, PlayerController->GetPawn());

        GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, End, ECollisionChannel::ECC_Visibility, TraceParams);

        if (HitDelegate.IsBound()) {
            HitDelegate.Broadcast(HitResult);
        }
    }
}