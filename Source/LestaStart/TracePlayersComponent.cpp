#include "TracePlayersComponent.h"
#include "Net/UnrealNetwork.h"

UTracePlayersComponent::UTracePlayersComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicatedByDefault(true);

    MaxLengthOfTrace = 100000.0;
}

void UTracePlayersComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTracePlayersComponent, MaxLengthOfTrace);
    //DOREPLIFETIME(UTracePlayersComponent, HitDelegate);
}

void UTracePlayersComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTracePlayersComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!GetOwner()->HasAuthority())return;

    if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
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