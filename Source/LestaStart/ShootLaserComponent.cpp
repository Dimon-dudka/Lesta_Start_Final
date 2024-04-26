#include "ShootLaserComponent.h"
#include "Core/LestaCharacter.h"

UShootLaserComponent::UShootLaserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Damage = 10.0;
	MaxDamageLen = 200.0;

	IsLazerWeapon = true;
	ShootingFlag = false;
}

void UShootLaserComponent::Shoot(FHitResult Hit) {
	HitRes = Hit;
	ShootingFlag = true;
}

void UShootLaserComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShootLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsLazerWeapon)return;

	if (ShootingFlag) {
		auto User = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);


		if (HitRes.Distance <= MaxDamageLen) {
			if (HitRes.GetActor() == User) {
				ALestaCharacter* ActorTmp = Cast<ALestaCharacter>(HitRes.GetActor());
				
				ActorTmp->GetDamage(Damage * DeltaTime);
				DrawDebugLine(GetWorld(), HitRes.TraceStart, HitRes.TraceEnd, FColor::Blue, false, 5, 1);
			}
			else if (ABoxActor* BoxActor = Cast<ABoxActor>(HitRes.GetActor())) {

				if (FVector::Dist(HitRes.TraceStart, User->GetActorLocation())<=MaxDamageLen) {

					FHitResult ActorHit;

					//	Solving problem with a few boxes between player and enemy

					bool CheckFromPawn = GetWorld()->LineTraceSingleByChannel(ActorHit,
						User->GetActorLocation(), BoxActor->GetActorLocation(), ECC_Visibility);
					
					if (ActorHit.GetActor() == BoxActor) {
						BoxActor->GetDamage(Damage * DeltaTime);
						DrawDebugLine(GetWorld(), HitRes.TraceStart, HitRes.TraceEnd, FColor::Blue, false, 5, 1);
					}
					else if(ABoxActor* SecondBoxActor = Cast<ABoxActor>(ActorHit.GetActor())){
						BoxActor->GetDamage(Damage * DeltaTime);
						DrawDebugLine(GetWorld(), HitRes.TraceStart, HitRes.TraceEnd, FColor::Blue, false, 5, 1);
					}
				}
			}
		}
		ShootingFlag = false;
	}
}