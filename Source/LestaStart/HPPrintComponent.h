//	Shows HP above actor to is attached

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "HPPrintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UHPPrintComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UHPPrintComponent();

	UFUNCTION()
		void SetupHealthPoints(double HP);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		double HeighAboveActor;

	UPROPERTY(EditAnywhere)
		double TextSize;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UTextRenderComponent> TextRender;	
};