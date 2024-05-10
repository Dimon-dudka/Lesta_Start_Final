//	Enemy trace component

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TraceEnemiesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UTraceEnemiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTraceEnemiesComponent();

	//	Return result of player trace
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTracedResult, FHitResult, Hit);
	UPROPERTY(Replicated)
	FTracedResult TraceResult;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};