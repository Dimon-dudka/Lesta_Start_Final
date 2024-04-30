//	Implement spawner actor, thst spawn an object by overlaping itself

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnPoint.h"
#include "SpawnerComponent.h"
#include "SpawnerActor.generated.h"

UCLASS()
class LESTASTART_API ASpawnerActor : public AActor
{
	GENERATED_BODY()

private:
	bool SpawnFlag;
	
public:	
	ASpawnerActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpawnerComponent* Spawner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USpawnPoint* SpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION()
		FTransform SpawnPointTransform();

	UFUNCTION()
		void OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};