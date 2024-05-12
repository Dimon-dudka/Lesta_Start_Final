//	Player Trace Component
//	Trace from center of view to the point on map

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TracePlayersComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UTracePlayersComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTracePlayersComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnHitDelegate, FHitResult, HitResult);
	FReturnHitDelegate HitDelegate;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere, Replicated)
		double MaxLengthOfTrace;
};