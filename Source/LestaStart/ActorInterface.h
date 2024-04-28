// Basic Actor interface

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActorInterface : public UInterface
{
	GENERATED_BODY()
};

class LESTASTART_API IActorInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
		virtual void GetDamage(const double& Damage) = 0;
};