//	Enemy trace component

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTraceComponent();

	//	Return result of player trace
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTracedResult, FHitResult, Hit);
	FTracedResult TraceResult;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;
};