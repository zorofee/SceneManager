// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneManager.h"
#include "SceneManagerStyle.h"
#include "SceneManagerCommands.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "DelegateManager.h"
#include "../Widgets/SSceneManagerEntry.h"

static const FName SceneManagerTabName("SceneManager");

#define LOCTEXT_NAMESPACE "FSceneManagerModule"

void FSceneManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSceneManagerStyle::Initialize();
	FSceneManagerStyle::ReloadTextures();

	FSceneManagerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSceneManagerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSceneManagerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSceneManagerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SceneManagerTabName, FOnSpawnTab::CreateRaw(this, &FSceneManagerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSceneManagerTabTitle", "SceneManager"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);


	DelegateManager::Get()->OnMatScalarValueChanged.AddRaw(this, &FSceneManagerModule::TestDelegate);

}

void FSceneManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSceneManagerStyle::Shutdown();

	FSceneManagerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SceneManagerTabName);
}

TSharedRef<SDockTab> FSceneManagerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSceneManagerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SceneManager.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.Padding(4)
			[
				SNew(SSceneManagerTools)
			]
		];
}


void FSceneManagerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SceneManagerTabName);
}

void FSceneManagerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSceneManagerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSceneManagerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FSceneManagerModule::TestDelegate(FString name, float value)
{
	UE_LOG(LogTemp, Warning, TEXT("TestDelegate %s %f"), *name, value);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSceneManagerModule, SceneManager)