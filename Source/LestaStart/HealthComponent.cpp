#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	Health = 100.0;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, GetHPValue);
	DOREPLIFETIME(UHealthComponent, GetNullHP);
}

void UHealthComponent::BecomeDamage_Implementation(const double& BecomeHPDamage) {

	if (!GetOwner()->HasAuthority())return;

	Health -= BecomeHPDamage;

	if (Health <= 0.0) {
		if (GetNullHP.IsBound()) {
			GetNullHP.Broadcast();
		}
	}
	else {
		if (GetHPValue.IsBound()) {
			GetHPValue.Broadcast(Health);
		}
	}
}

double UHealthComponent::GetHP() const {
	return Health;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetHPValue.IsBound()) {
		GetHPValue.Broadcast(Health);
	}
}

void UHealthComponent::SetHP_Implementation(const double& HP) {
	Health = HP;
}