#include "SpawnerActor.h"
#include "Net/UnrealNetwork.h"

ASpawnerActor::ASpawnerActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SpawnFlag = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(Box);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASpawnerActor::OverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	Mesh->SetupAttachment(RootComponent);

	Spawner = CreateDefaultSubobject<USpawnerComponent>(TEXT("Spawner Component"));
	Spawner->GetSpawnTransform.BindDynamic(this, &ASpawnerActor::SpawnPointTransform);

	SpawnPoint = CreateDefaultSubobject<USpawnPoint>(TEXT("Spawn Point Component"));
	SpawnPoint->SetupAttachment(RootComponent);
}

void ASpawnerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnerActor, SpawnFlag);
	DOREPLIFETIME(ASpawnerActor, Spawner);
	DOREPLIFETIME(ASpawnerActor, SpawnPoint);
	DOREPLIFETIME(ASpawnerActor, Box);
}

void ASpawnerActor::OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!HasAuthority())return;

	if (!SpawnFlag) {
		SpawnFlag = true;
		Spawner->SpawningActor();
		Destroy();
	}
}

FTransform ASpawnerActor::SpawnPointTransform() {
	return SpawnPoint->GetComponentTransform();
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetIsReplicated(true);
}

void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}