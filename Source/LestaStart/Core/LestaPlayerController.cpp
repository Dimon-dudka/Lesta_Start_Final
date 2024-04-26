#include "LestaPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ALestaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(InputMapping, InputMappingPriority);
		}
	}
}