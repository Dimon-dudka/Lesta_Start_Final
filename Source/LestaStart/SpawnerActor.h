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

	//	Is actor spawned yet
	UPROPERTY(Replicated)
		bool SpawnFlag;
	
public:	
	ASpawnerActor();

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<USpawnerComponent> Spawner;

	UPROPERTY(EditAnywhere, Replicated)
		TObjectPtr<USpawnPoint> SpawnPoint;

	UPROPERTY(EditDefaultsOnly, Replicated)
		TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> Mesh;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	virtual void BeginPlay() override;

	UFUNCTION()
		FTransform SpawnPointTransform();

	UFUNCTION()
		void OverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};