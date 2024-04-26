#include "DestroyAnimationComponent.h"

UDestroyAnimationComponent::UDestroyAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DestroyFlag = false;
	CurrentRadius = 0.0;
	ExplosionRadius = 200.0;
}

void UDestroyAnimationComponent::StartAnimation() {
	DestroyFlag = true;
}


void UDestroyAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UDestroyAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DestroyFlag) {
		if (CurrentRadius > ExplosionRadius) {
			DestroyFlag = false;
			if (AnimationStatus.IsBound()) {
				AnimationStatus.Broadcast();
			}
		}
		else {
			CurrentRadius += 10.0;
			DrawDebugSphere(GetWorld(), GetComponentLocation(), CurrentRadius, 20, FColor::Red, 0, 0.1);
		}
	}
}