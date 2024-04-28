#include "BoxActor.h"

ABoxActor::ABoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->GetNullHP.AddDynamic(this, &ABoxActor::GetNullHPInfo);

	DestroyAnimation = CreateDefaultSubobject<UDestroyAnimationComponent>(TEXT("Destroy Animation"));
	DestroyAnimation->SetupAttachment(RootComponent);
	DestroyAnimation->AnimationStatus.AddDynamic(this, &ABoxActor::GetDestroyed);

}

void ABoxActor::GetDestroyed() {
	Destroy();
}

void ABoxActor::GetNullHPInfo() {
	DestroyAnimation->SetWorldLocation(Mesh->GetComponentLocation());
	DestroyAnimation->StartAnimation();
	Mesh->DestroyComponent();
}

void ABoxActor::GetDamage(const double& Damage) {
	Health->BecomeDamage(Damage);
}

void ABoxActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}