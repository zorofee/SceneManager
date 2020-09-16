// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../Widgets/SSceneManagerEntry.h"
#include "DataStructures.h"
#include "SceneManagerSaveGame.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSceneManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<SSceneManagerTools> SceneManagerTools;

	USceneManagerSaveGame* saveGame;

	void AddPlanData(FString planName);
	void AddMatGroupData(TSharedPtr<FMaterialGroupInfo> groupInfo);
	void AddMatInstanceData(TSharedPtr<FMaterialInfo> matInfo);
	void SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo);


	void DeleteMatInstance(TSharedPtr<FMaterialInfo> matInfo);


	void SaveGameData();
	void LoadGameData();
};
