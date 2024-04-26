#include "DoorActor.h"

ADoorActor::ADoorActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	OpeningArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Opening Area"));
	SetRootComponent(OpeningArea);
	OpeningArea->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OpenDoor);
	OpeningArea->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::CloseDoor);

	DoorMove = CreateDefaultSubobject<UDoorMovementComponent>(TEXT("Door Movement"));
	DoorMove->DoorPos.AddDynamic(this, &ADoorActor::MoveDoor);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ADoorActor::MoveDoor(FVector NewDoorPos) {
	Mesh->SetRelativeLocation(NewDoorPos);
}

void ADoorActor::OpenDoor(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	DoorMove->OpenClose(1);
}

void ADoorActor::CloseDoor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	DoorMove->OpenClose(0);
}


void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	DoorMove->InitialSetup(Mesh->GetRelativeLocation());
}

void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}