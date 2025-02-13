// Copyright Epic Games, Inc. All Rights Reserved.

#include "pj8GameMode.h"
#include "Characters/pj8Character.h"
#include "UObject/ConstructorHelpers.h"

Apj8GameMode::Apj8GameMode()
{
	// Because I use GameMode and Other Setting Classes wrapped BP, below code is useless.
	
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
	// PlayerControllerClass = ASpartaPlayerController::StaticClass();
	// DefaultPawnClass = ASpartaCharacter::StaticClass();
	// GameStateClass = ASpartaGameStateBase::StaticClass();
}
