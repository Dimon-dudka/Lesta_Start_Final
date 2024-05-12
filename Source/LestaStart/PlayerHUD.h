//	Implements custom user HUD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "PlayerHUD.generated.h"

UCLASS()
class LESTASTART_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	APlayerHUD();

	UFUNCTION()
		void SetReloadingText(bool flag);

	UFUNCTION()
		void SetTextHP(double HP);

	UFUNCTION()
		void SetTextWeapon(FString Weapon);

protected:
	
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

private:
	UPROPERTY(EditAnywhere)
		FString HPText;

	UPROPERTY(EditAnywhere)
		FString WeaponText;

	UPROPERTY(EditAnywhere)
		FString ReloadingText;
};