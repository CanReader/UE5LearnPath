// All copyrights are reserved by Canberk Pitirli

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE5_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	void PostLogin(APlayerController* NewPlayer) override;
	
};
