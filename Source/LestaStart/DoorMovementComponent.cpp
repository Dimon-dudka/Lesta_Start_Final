#include "DoorMovementComponent.h"

UDoorMovementComponent::UDoorMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentPos = 0.0;
	StatusFlag = false;
	MaxLen = 70.0;
	SpeedKoef = 50.0;
}

void UDoorMovementComponent::InitialSetup(const FVector& pos) {
	ActorInitialPosition = pos;
	ActorCurrentPosition = pos;
}

void UDoorMovementComponent::OpenClose(const bool& DoorStatus) {
	StatusFlag = DoorStatus;
}

void UDoorMovementComponent::SetupLengthOfDoorMove(const double& Length) {
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