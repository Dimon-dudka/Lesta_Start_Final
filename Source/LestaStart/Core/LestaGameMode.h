// Game Mode Class

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include"../PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

#include "LestaGameMode.generated.h"

UCLASS()
class LESTASTART_API ALestaGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	TObjectPtr<APlayerHUD>UserHUD;

	virtual void BeginPlay() override;
};