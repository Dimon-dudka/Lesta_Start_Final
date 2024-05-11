#include "BoxActor.h"
#include "Net/UnrealNetwork.h"

ABoxActor::ABoxActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

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

void ABoxActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoxActor, Health);
	DOREPLIFETIME(ABoxActor, Box);
	DOREPLIFETIME(ABoxActor, Mesh);
}

void ABoxActor::GetDestroyed_Implementation() {

	if (!HasAuthority())return;

	Destroy();
}

void ABoxActor::GetNullHPInfo_Implementation() {

	DestroyAnimation->SetWorldLocation(Mesh->GetComponentLocation());
	DestroyAnimation->StartAnimation();

	if (!HasAuthority())return;
	Mesh->DestroyComponent();
}

void ABoxActor::GetDamage(const double& Damage) {
	if (!HasAuthority())return;

	Health->BecomeDamage(Damage);
}

void ABoxActor::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetIsReplicated(true);
}

void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}