#include "LestaGameMode.h"
#include "Kismet/GameplayStatics.h"


void ALestaGameMode::BeginPlay() {
	Super::BeginPlay();

	bUseSeamlessTravel = true;

	ActivePlayers = GetNumPlayers();
}

void ALestaGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	ActivePlayers -= 1;
}

void ALestaGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	ActivePlayers += 1;

	if (!NewPlayer->GetPawn())return;

	NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &ALestaGameMode::DeactivatePlayer);
}

void ALestaGameMode::RestartGame() {

	ActivePlayers = 0;
	
	GetWorld()->ServerTravel(GetWorld()->GetMapName());
}

void ALestaGameMode::DeactivatePlayer(AActor* DestroyedActor) {
	
	ActivePlayers -= 1;

	if (ActivePlayers <= 0) {
		RestartGame();
	}
}