//	Describes the way the door changes position while opene/close

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoorMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UDoorMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	double SpeedKoef;

public:	
	UDoorMovementComponent();

	//	Return of the calculated new door position
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDoorPosition, FVector, VectorCoord);
	FDoorPosition DoorPos;

	//	Setting door status [open, close]
	UFUNCTION()
		void OpenClose(const bool& DoorStatus);

	UFUNCTION()
		void InitialSetup(const FVector& pos);

	UFUNCTION()
		void SetupLengthOfDoorMove(const double& Length);

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		FVector ActorInitialPosition;

	UPROPERTY(EditDefaultsOnly)
		FVector ActorCurrentPosition;

	UPROPERTY(EditAnywhere)
		double MaxLen;

	double CurrentPos;

	bool StatusFlag;
};