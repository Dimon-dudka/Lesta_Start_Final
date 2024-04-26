#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LestaPlayerController.generated.h"

class UInputMappingContext;

/** Base Player Controller class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Added input mapping context on startup. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	/** Priority of InputMapping. */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (ClampMin = 0))
	int32 InputMappingPriority = 0;

	virtual void BeginPlay() override;
};