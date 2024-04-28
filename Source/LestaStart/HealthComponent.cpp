#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = 100.0;
}

void UHealthComponent::BecomeDamage(const double& BecomeHPDamage) {
	Health -= BecomeHPDamage;

	if (Health <= 0.0) {
		if (GetNullHP.IsBound()) {
			GetNullHP.Broadcast();
		}
	}
}

double  UHealthComponent::GetHP() const {
	return Health;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::SetHP(const double& HP) {
	Health = HP;
}