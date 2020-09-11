// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SceneManagerStyle.h"

class FSceneManagerCommands : public TCommands<FSceneManagerCommands>
{
public:

	FSceneManagerCommands()
		: TCommands<FSceneManagerCommands>(TEXT("SceneManager"), NSLOCTEXT("Contexts", "SceneManager", "SceneManager Plugin"), NAME_None, FSceneManagerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};