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

	UPROPERTY(EditAnywhere, Replicated)
	double SpeedKoef;

public:	
	UDoorMovementComponent();

	//	Return of the calculated new door position
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDoorPosition, FVector, VectorCoord);
	UPROPERTY(Replicated)
	FDoorPosition DoorPos;

	//	Setting door status [open, close]
	UFUNCTION(Server, Unreliable)
		void OpenClose(bool DoorStatus);

	UFUNCTION(Server, Unreliable)
		void InitialSetup(const FVector& pos);

	UFUNCTION(Server, Unreliable)
		void SetupLengthOfDoorMove(const double& Length);

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Replicated)
		FVector ActorInitialPosition;

	UPROPERTY(EditDefaultsOnly, Replicated)
		FVector ActorCurrentPosition;

	UPROPERTY(EditAnywhere, Replicated)
		double MaxLen;

	UPROPERTY(Replicated)
	double CurrentPos;

	UPROPERTY(Replicated)
	bool StatusFlag;
};