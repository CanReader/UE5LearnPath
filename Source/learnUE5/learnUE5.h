// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

void DebugMessage(FString text, FColor color = FColor::Red) {

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, 
			color,
			text
		);
	}
}