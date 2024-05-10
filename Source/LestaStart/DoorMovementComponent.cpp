#include "DoorMovementComponent.h"
#include "Net/UnrealNetwork.h"

UDoorMovementComponent::UDoorMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	CurrentPos = 0.0;
	StatusFlag = false;
	MaxLen = 70.0;
	SpeedKoef = 50.0;
}

void UDoorMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDoorMovementComponent, SpeedKoef);
	DOREPLIFETIME(UDoorMovementComponent, ActorInitialPosition);
	DOREPLIFETIME(UDoorMovementComponent, ActorCurrentPosition);
	DOREPLIFETIME(UDoorMovementComponent, MaxLen);
	DOREPLIFETIME(UDoorMovementComponent, CurrentPos);
	DOREPLIFETIME(UDoorMovementComponent, StatusFlag);
	DOREPLIFETIME(UDoorMovementComponent, DoorPos);
}

void UDoorMovementComponent::InitialSetup_Implementation(const FVector& pos) {
	ActorInitialPosition = pos;
	ActorCurrentPosition = pos;
}

void UDoorMovementComponent::OpenClose_Implementation(bool DoorStatus) {
	StatusFlag = DoorStatus;
}

void UDoorMovementComponent::SetupLengthOfDoorMove_Implementation(const double& Length) {
	MaxLen = Length;
}


void UDoorMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDoorMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StatusFlag) {
		
		if (CurrentPos < MaxLen) {
			CurrentPos += SpeedKoef*DeltaTime;

			if (CurrentPos > MaxLen) {
				CurrentPos = MaxLen;
				ActorCurrentPosition.X = ActorInitialPosition.X + MaxLen;
			}
			else {
				ActorCurrentPosition.X += SpeedKoef * DeltaTime;
			}

			if (DoorPos.IsBound()) {
				DoorPos.Broadcast(ActorCurrentPosition);
			}
		}
	}
	else {
		
		if (CurrentPos > 0.0) {
			CurrentPos -= SpeedKoef * DeltaTime;
			
			if (CurrentPos < 0.0) {
				CurrentPos = 0.0;
				ActorCurrentPosition.X = ActorInitialPosition.X;
			}
			else {
				ActorCurrentPosition.X -= SpeedKoef * DeltaTime;
			}

			if (DoorPos.IsBound()) {
				DoorPos.Broadcast(ActorCurrentPosition);
			}
		}
	}	
}