// Copyright Epic Games, Inc. All Rights Reserved.

#include "SceneManager.h"
#include "SceneManagerStyle.h"
#include "SceneManagerCommands.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "DelegateManager.h"
#include "Kismet/GameplayStatics.h"
#include "DelegateManager.h"



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




	if(!DelegateManager::Get()->AddSceneMatPlan.IsBound())
		DelegateManager::Get()->AddSceneMatPlan.AddRaw(this, &FSceneManagerModule::AddPlanData);
	
	if (!DelegateManager::Get()->AddSceneMatGroup.IsBound())
		DelegateManager::Get()->AddSceneMatGroup.AddRaw(this, &FSceneManagerModule::AddMatGroupData);
	
	if (!DelegateManager::Get()->AddSceneMatInstance.IsBound())
		DelegateManager::Get()->AddSceneMatInstance.AddRaw(this, &FSceneManagerModule::AddMatInstanceData);
	
	if (!DelegateManager::Get()->OnMatParamChanged.IsBound())
		DelegateManager::Get()->OnMatParamChanged.AddRaw(this, &FSceneManagerModule::SetMatScalarParam);
	
	if (!DelegateManager::Get()->SaveGameData.IsBound())
		DelegateManager::Get()->SaveGameData.AddRaw(this, &FSceneManagerModule::SaveGameData);
	else
	{
		DelegateManager::Get()->SaveGameData.Clear();
		DelegateManager::Get()->SaveGameData.AddRaw(this, &FSceneManagerModule::SaveGameData);
	}
	
	if (!DelegateManager::Get()->LoadGameData.IsBound())
		DelegateManager::Get()->LoadGameData.AddRaw(this, &FSceneManagerModule::LoadGameData);

	if (!DelegateManager::Get()->DeleteSceneMatGroup.IsBound())
		DelegateManager::Get()->DeleteSceneMatGroup.AddRaw(this, &FSceneManagerModule::DeleteMatGroup);

	if (!DelegateManager::Get()->DeleteSceneMatInstance.IsBound())
		DelegateManager::Get()->DeleteSceneMatInstance.AddRaw(this, &FSceneManagerModule::DeleteMatInstance);

	if (!DelegateManager::Get()->ReplaceSceneMatInstance.IsBound())
		DelegateManager::Get()->ReplaceSceneMatInstance.AddRaw(this, &FSceneManagerModule::ReplaceMatInstance);
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
	saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
	
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
				SAssignNew(SceneManagerTools,SSceneManagerTools)
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











void FSceneManagerModule::AddPlanData(FString planName)
{

	FMaterialPlanInfo planData;
	planData.Name = planName;
	saveGame->PlanList.Emplace(planName, planData);
}


void FSceneManagerModule::AddMatGroupData(TSharedPtr<FMaterialGroupInfo> groupInfo)
{

	if (saveGame->PlanList.Contains(groupInfo->Parent))
	{
		FMaterialGroupInfo groupData;
		groupData.GroupName = groupInfo->GroupName;
		groupData.Parent = groupInfo->Parent;
		saveGame->PlanList[groupInfo->Parent].GroupList.Emplace(groupData.GroupName, groupData);
	}
}

void FSceneManagerModule::AddMatInstanceData(TSharedPtr<FMaterialInfo> matInfo)
{

	if (saveGame->PlanList.Contains(matInfo->ParentPlan))
	{
		if (saveGame->PlanList[matInfo->ParentPlan].GroupList.Contains(matInfo->ParentGroup))
		{
			FMaterialInfo matData;
			matData.MatPath = matInfo->MatPath;
			matData.ParentGroup = matInfo->ParentGroup;
			matData.ParentPlan = matInfo->ParentPlan;
			matData.ScalarParams = matInfo->ScalarParams;
			matData.VectorParams = matInfo->VectorParams;
			saveGame->PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Emplace(matInfo->MatPath, matData);
		}
	}
}

void FSceneManagerModule::SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo)
{
	FString plan = matInfo->ParentPlan;
	FString group = matInfo->ParentGroup;
	FString path = matInfo->MatPath;
	if (saveGame->PlanList.Contains(plan))
	{
		if (saveGame->PlanList[plan].GroupList.Contains(group))
		{
			if (saveGame->PlanList[plan].GroupList[group].MatList.Contains(path))
			{
				saveGame->PlanList[plan].GroupList[group].MatList[path] = *matInfo;
			}
		}
	}
}

void FSceneManagerModule::DeleteMatGroup(TSharedPtr<FMaterialGroupInfo> matInfo)
{
	FString plan = matInfo->Parent;
	FString group = matInfo->GroupName;
	if (saveGame->PlanList.Contains(plan))
	{
		if (saveGame->PlanList[plan].GroupList.Contains(group))
		{
			saveGame->PlanList[plan].GroupList.Remove(group);

		}
	}

	/*
	重新刷新
	*/

	//SceneManagerTools->ClearChildren();
	SceneManagerTools->MatGroupItems.Empty();
	SceneManagerTools->ListView->RequestListRefresh();

	for (TPair<FString, FMaterialPlanInfo> iterator : saveGame->PlanList)
	{
		for (TPair<FString, FMaterialGroupInfo> groupIt : iterator.Value.GroupList)
		{
			if (SceneManagerTools)
			{
				SceneManagerTools->AddMaterialGroup(groupIt.Value);
			}
		}
	}
}


void FSceneManagerModule::DeleteMatInstance(TSharedPtr<FMaterialInfo> matInfo)
{
	FString plan = matInfo->ParentPlan;
	FString group = matInfo->ParentGroup;
	FString path = matInfo->MatPath;
	if (saveGame->PlanList.Contains(plan))
	{
		if (saveGame->PlanList[plan].GroupList.Contains(group))
		{
			if (saveGame->PlanList[plan].GroupList[group].MatList.Contains(path))
			{
				saveGame->PlanList[plan].GroupList[group].MatList.Remove(path);
			}
		}
	}

	/*
	在切换材质球时只需要清空数据
	
	在删除材质球时需要调下面的方法
	*/

	SceneManagerTools->MatGroupItems.Empty();
	SceneManagerTools->ListView->RequestListRefresh();
	
	for (TPair<FString, FMaterialPlanInfo> iterator : saveGame->PlanList)
	{
		for (TPair<FString, FMaterialGroupInfo> groupIt : iterator.Value.GroupList)
		{
			if (SceneManagerTools)
			{
				SceneManagerTools->AddMaterialGroup(groupIt.Value);
			}
		}
	}

}

void FSceneManagerModule::ReplaceMatInstance(TSharedPtr<FMaterialInfo> matInfo, FString originPath)
{
	UE_LOG(LogTemp, Warning, TEXT("REMOVE 11 %d"), matInfo->ScalarParams.Num());
	FString plan = matInfo->ParentPlan;
	FString group = matInfo->ParentGroup;
	FString path = matInfo->MatPath;
	if (saveGame->PlanList.Contains(plan))
	{
		if (saveGame->PlanList[plan].GroupList.Contains(group))
		{
			if (saveGame->PlanList[plan].GroupList[group].MatList.Contains(originPath))
			{
				saveGame->PlanList[plan].GroupList[group].MatList.Remove(originPath);
			}
		}
	}

	AddMatInstanceData(matInfo);
}


void FSceneManagerModule::SaveGameData()
{
	if (saveGame)
	{
		UGameplayStatics::SaveGameToSlot(saveGame, TEXT("TestSlot"), 0);
	}
}

void FSceneManagerModule::LoadGameData()
{
	saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));

	for (TPair<FString, FMaterialPlanInfo> plan : saveGame->PlanList)
	{
		for (TPair<FString, FMaterialGroupInfo> group : plan.Value.GroupList)
		{
			if (SceneManagerTools)
			{
				SceneManagerTools->AddMaterialGroup(group.Value);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSceneManagerModule, SceneManager)