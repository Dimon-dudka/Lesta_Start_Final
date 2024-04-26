#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.0;
}

void UHealthComponent::BecomeDamage(const double& BecomeHPDamage) {
	Health -= BecomeHPDamage;
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

	if (Health <= 0.0) {
		if (GetNullHP.IsBound()) {
			GetNullHP.Broadcast();
		}
	}
}

void UHealthComponent::SetHP(const double& HP) {
	Health = HP;
}