#include "LazerShootUserComponent.h"

ULazerShootUserComponent::ULazerShootUserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ShootingFlag = false;

    Damage = 20;
    MaxLength = 2000;

    MaxShootingTime = 5.0;
    CurrentShootingTime = 0.0;

    IsReloading = false;
    ReloadingTime = 2.0;
    CurrentReloadingTime = 0.0;
}

void ULazerShootUserComponent::StartReload() {
    IsReloading = true;
}

void ULazerShootUserComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULazerShootUserComponent::GetShooting(const bool& Shooting) {
	ShootingFlag = Shooting;
}


void ULazerShootUserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	if (!ShootingFlag||CurrentShootingTime>=MaxShootingTime) return;

    if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        FVector ViewLocation;
        FRotator ViewRotation;
        PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

        FVector End = ViewLocation + (ViewRotation.Vector() * MaxLength); 

        FHitResult HitResult;
        FCollisionQueryParams TraceParams(FName(TEXT("TraceFromPlayerView")), true, PlayerController->GetPawn());

        GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, End, ECollisionChannel::ECC_Visibility, TraceParams);

        CurrentShootingTime += DeltaTime;

        if (HitResult.bBlockingHit)
        {
            DrawDebugLine(GetWorld(), ViewLocation, HitResult.ImpactPoint, FColor::Green, false, 0.001f, 0, 1.f);

            if (HitResult.Distance > MaxLength) return;

            AActor* HitActor = HitResult.GetActor();
            if (HitActor && HitActor->IsA(ABoxActor::StaticClass())) {
                ABoxActor* BoxTmp = Cast<ABoxActor>(HitResult.GetActor());
                BoxTmp->GetDamage(Damage * DeltaTime);
            }
            else if (HitActor && HitActor->IsA(AEnemyActorBasic::StaticClass())) {
                AEnemyActorBasic* EnemyTmp = Cast<AEnemyActorBasic>(HitResult.GetActor());
                EnemyTmp->GetDamage(Damage * DeltaTime);
            }

        }
        else
        {
            DrawDebugLine(GetWorld(), ViewLocation, End, FColor::Green, false, 0.001f, 0, 1.f);
        }
    }
}