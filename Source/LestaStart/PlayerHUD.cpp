#include "PlayerHUD.h"
#include "Core/LestaCharacter.h"

APlayerHUD::APlayerHUD() {

	PrimaryActorTick.bCanEverTick = false;

	HPText = { "NONE" };
	WeaponText = { "NONE" };
	ReloadingText = { "" };

}

void APlayerHUD::DrawHUD() {

	Super::DrawHUD();

	DrawText(HPText, FLinearColor::Red, 0, 0, nullptr, 2, false);
	DrawText(WeaponText, FLinearColor::Red, 0, 25, nullptr, 2, false);
	DrawText(ReloadingText, FLinearColor::Red, 0, 50, nullptr, 2, false);
}

void APlayerHUD::SetTextHP(double HP) {
	HPText = LexToSanitizedString((static_cast<int>(HP) / 5) * 5) + " HP";
}

void APlayerHUD::SetTextWeapon(FString Weapon) {
	WeaponText = Weapon;
}

void APlayerHUD::SetReloadingText(bool flag) {
	if (flag)ReloadingText = "Reloading";
	else ReloadingText = "";
}

void APlayerHUD::BeginPlay() {
	Super::BeginPlay();
}
