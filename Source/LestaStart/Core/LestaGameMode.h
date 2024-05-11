// Game Mode Class

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LestaGameMode.generated.h"

UCLASS()
class LESTASTART_API ALestaGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	UFUNCTION()
		void RestartGame();

	UFUNCTION()
		void DeactivatePlayer(AActor* DestroyedActor);

	int32 ActivePlayers;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;
};