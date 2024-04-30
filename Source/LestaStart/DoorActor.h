//	Describes the door actor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DoorMovementComponent.h"
#include "DoorActor.generated.h"

UCLASS()
class LESTASTART_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoorActor();

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Mesh;

	//	Area within starting overlaps to open/close the door
	UPROPERTY(EditAnywhere)
		TObjectPtr<UBoxComponent> OpeningArea;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UDoorMovementComponent> DoorMove;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//	Update Dooor position
	UFUNCTION()
		void MoveDoor(FVector NewDoorPos);

	UFUNCTION()
		void OpenDoor(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CloseDoor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};