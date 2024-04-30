#include "LazerShootComponent.h"

ULazerShootComponent::ULazerShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    ShootingFlag = false;

    IsPlayer = true;
    MustShoot = true;

    Damage = 20;
    MaxLength = 2000;

    MaxShootingTime = 5.0;
    CurrentShootingTime = 0.0;

    IsReloading = false;
    ReloadingTime = 2.0;
    CurrentReloadingTime = 0.0;
}


void ULazerShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULazerShootComponent::ShootStatus(bool Flag) {
    ShootingFlag = Flag;
}

void ULazerShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!MustShoot)return;

    if (IsReloading) {

        CurrentReloadingTime += DeltaTime;

        if (CurrentReloadingTime >= ReloadingTime) {
            CurrentReloadingTime = 0.0;
            CurrentShootingTime = 0.0;
            IsReloading = false;

            if (IsReload.IsBound()) {
                IsReload.Broadcast();
            }
        }
        return;
    }

    if (!ShootingFlag || CurrentShootingTime >= MaxShootingTime || !Hit.IsValidBlockingHit()) return;

    if(IsPlayer)CurrentShootingTime += DeltaTime;

    if (Hit.Distance > MaxLength) return;

    DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.ImpactPoint, FColor::Green, false, 0.001f, 0, 1.f);

    if (Hit.bBlockingHit)
    {
        IActorInterface* ActorInterfaceTmp = Cast<IActorInterface>(Hit.GetActor());

        if (!ActorInterfaceTmp)return;

        ActorInterfaceTmp->GetDamage(Damage * DeltaTime);
    }
}


void ULazerShootComponent::GetHit(FHitResult HitRes) {
	Hit = HitRes;
}


void ULazerShootComponent::StartReload() {
    IsReloading = true;
}