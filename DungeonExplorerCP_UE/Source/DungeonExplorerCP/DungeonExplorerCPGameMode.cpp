// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DungeonExplorerCPGameMode.h"
#include "DungeonExplorerCPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADungeonExplorerCPGameMode::ADungeonExplorerCPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
