#include "DoorActor.h"
#include "Net/UnrealNetwork.h"

ADoorActor::ADoorActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	NetUpdateFrequency = 120;
	SetReplicateMovement(true);

	OpeningArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Opening Area"));
	SetRootComponent(OpeningArea);
	OpeningArea->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OpenDoor);
	OpeningArea->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::CloseDoor);

	DoorMove = CreateDefaultSubobject<UDoorMovementComponent>(TEXT("Door Movement"));
	DoorMove->DoorPos.AddDynamic(this, &ADoorActor::MoveDoor);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoorActor, OpeningArea);
	DOREPLIFETIME(ADoorActor, DoorMove);
}

void ADoorActor::MoveDoor_Implementation(FVector NewDoorPos) {

	if (!HasAuthority())return;

	Mesh->SetRelativeLocation(NewDoorPos);
}

void ADoorActor::OpenDoor_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (!HasAuthority())return;
	
	DoorMove->OpenClose(1);
}

void ADoorActor::CloseDoor_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (!HasAuthority())return;

	DoorMove->OpenClose(0);
}


void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	DoorMove->InitialSetup(Mesh->GetRelativeLocation());
	Mesh->SetIsReplicated(true);
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}