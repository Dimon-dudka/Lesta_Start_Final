#include "LestaGameMode.h"

void ALestaGameMode::BeginPlay() {

	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		UserHUD = GetWorld()->SpawnActor<APlayerHUD>();
		PlayerController->MyHUD = UserHUD;
	}
}