// Copyright Epic Games, Inc. All Rights Reserved.

#include "learnUE5GameMode.h"
#include "learnUE5Character.h"
#include "UObject/ConstructorHelpers.h"

AlearnUE5GameMode::AlearnUE5GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
