// Copyright Epic Games, Inc. All Rights Reserved.

#include "learnUE5GameMode.h"
#include "learnUE5Character.h"
#include "UObject/ConstructorHelpers.h"

AlearnUE5GameMode::AlearnUE5GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_LearnUE5Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
