//	Provides a destroy animation  component

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DestroyAnimationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LESTASTART_API UDestroyAnimationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UDestroyAnimationComponent();

	//	Animation over answer to owner
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndOfAnimation);
	FEndOfAnimation AnimationStatus;

	UFUNCTION()
		void StartAnimation();

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	//	Destroy animation max radius
	UPROPERTY(EditAnywhere)
		double ExplosionRadius;

	//	Speed of animation playing
	UPROPERTY(EditAnywhere)
		double AnimationSpeed;

	//	Is the animation over
	bool DestroyFlag;

	//	Current animation radius
	double CurrentRadius;

};