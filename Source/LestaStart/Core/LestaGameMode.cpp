#include "LestaGameMode.h"
#include "Kismet/GameplayStatics.h"


void ALestaGameMode::BeginPlay() {
	Super::BeginPlay();

	bUseSeamlessTravel = true;

	BasicMap = GetWorld();
}

void ALestaGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	//if (ActiveUsersTable[Exiting])ActivePlayers -= 1;

	//PlayerControllerList.Remove(Exiting);
}

void ALestaGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	
	UE_LOG(LogInit, Error, TEXT("New log"));

	PlayerControllerList.Add(NewPlayer);

	//NewPlayer->ClientTravel(BasicMap->GetAddressURL(), ETravelType::TRAVEL_Absolute);

	NewPlayer->GetPawn()->OnDestroyed.AddDynamic(this, &ALestaGameMode::DeactivatePlayer);
	//NewPlayer->OnPossessedPawnChanged.AddDynamic(this, &ALestaGameMode::DeactivatePlayer);

	ActivePlayers += 1;
}

void ALestaGameMode::RestartGame() {

	ActivePlayers = 0;
	//ActiveUsersTable.Empty();

	//for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {

		//if (It->IsValid()) {
			//RestartPlayer(Cast<AController>(*It));

			//It->Get()->ClientTravel(GetWorld()->GetAddressURL(), ETravelType::TRAVEL_Relative);

			//ActivePlayers += 1;
			//UE_LOG(LogInit, Error, TEXT("Restart Player: %d"), It->Get()->GetUniqueID());
		//}
	//}
	
	GetWorld()->ServerTravel(TEXT("../../../Content/Maps/LestaStartMap"));
}

void ALestaGameMode::DeactivatePlayer(AActor* DestroyedActor) {
	
	ActivePlayers -= 1;

	if (ActivePlayers <= 0) {
		UE_LOG(LogInit, Error, TEXT("Start of game restart"));
		RestartGame();
	}
}