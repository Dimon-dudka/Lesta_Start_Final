// Game Mode Class

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "LestaGameMode.generated.h"

UCLASS()
class LESTASTART_API ALestaGameMode : public AGameMode //, public AGameModeBase
{
	GENERATED_BODY()

private:

	virtual void RestartGame();

	UWorld* BasicMap;

	int32 ActivePlayers;

	UPROPERTY()
		TArray<APlayerController*> PlayerControllerList;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION()
	void DeactivatePlayer(AActor* DestroyedActor);

	virtual void BeginPlay() override;
};