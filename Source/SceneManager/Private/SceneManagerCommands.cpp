// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneManagerCommands.h"

#define LOCTEXT_NAMESPACE "FSceneManagerModule"

void FSceneManagerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SceneManager", "Bring up SceneManager window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
