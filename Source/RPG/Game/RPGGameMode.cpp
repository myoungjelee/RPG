// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"


ARPGGameMode::ARPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Character/Player/BP_RPGPlayer.BP_RPGPlayer_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
