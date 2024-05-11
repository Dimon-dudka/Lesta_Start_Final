#include "LazerShootComponent.h"
#include "Net/UnrealNetwork.h"

ULazerShootComponent::ULazerShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    SetIsReplicatedByDefault(true);

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


void ULazerShootComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ULazerShootComponent, ShootingFlag);
    DOREPLIFETIME(ULazerShootComponent, IsPlayer);
    DOREPLIFETIME(ULazerShootComponent, MustShoot);
    DOREPLIFETIME(ULazerShootComponent, Damage);
    DOREPLIFETIME(ULazerShootComponent, MaxLength);
    DOREPLIFETIME(ULazerShootComponent, MaxShootingTime);
    DOREPLIFETIME(ULazerShootComponent, CurrentShootingTime);
    DOREPLIFETIME(ULazerShootComponent, IsReloading);
    DOREPLIFETIME(ULazerShootComponent, ReloadingTime);
    DOREPLIFETIME(ULazerShootComponent, CurrentReloadingTime);
    DOREPLIFETIME(ULazerShootComponent, Hit);
    DOREPLIFETIME(ULazerShootComponent, IsReload);
}

void ULazerShootComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULazerShootComponent::ShootStatus_Implementation(bool Flag) {
    ShootingFlag = Flag;
}

void ULazerShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!MustShoot)return;

    if (IsReloading&& GetOwner()->HasAuthority()) {

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

    //  Only player has limited lazer
    if (IsPlayer)CurrentShootingTime += DeltaTime;

    if (Hit.Distance > MaxLength) return;

    DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.ImpactPoint, FColor::Green, false, 0.001f, 0, 1.f);

    if (!GetOwner()->HasAuthority())return;

    if (Hit.bBlockingHit)
    {
        IActorInterface* ActorInterfaceTmp = Cast<IActorInterface>(Hit.GetActor());

        if (!ActorInterfaceTmp)return;

        ActorInterfaceTmp->GetDamage(Damage * DeltaTime);
    }    
}


void ULazerShootComponent::GetHit_Implementation(FHitResult HitRes) {
	Hit = HitRes;
}


void ULazerShootComponent::StartReload_Implementation() {
    IsReloading = true;
}