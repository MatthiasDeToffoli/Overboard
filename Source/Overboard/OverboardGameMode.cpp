// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverboardGameMode.h"
#include "OverboardCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOverboardGameMode::AOverboardGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
