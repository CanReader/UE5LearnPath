#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

ALobbyGameMode::ALobbyGameMode() {
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	int32 TotalPlayers = GameState.Get()->PlayerArray.Num();

	if (TotalPlayers >= 2)
	{
		bUseSeamlessTravel = true;
		UWorld* world = GetWorld();
		if (world)
			world->ServerTravel(FString("/Game/Maps/TheGame?listen"));
	}
}
