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

	if (!DelegateManager::Get()->RefreshPlanList.IsBound())
		DelegateManager::Get()->RefreshPlanList.AddRaw(this, &FSceneManagerModule::RefreshPlanList);

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
	
	auto OnTabClosed = [this](TSharedRef<SDockTab>)
	{
		// Tab closed - leave snapshot mode
		SaveGameData();
	};

	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSceneManagerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SceneManager.cpp"))
		);



	TSharedPtr<SDockTab> dockTab;

	 SAssignNew(dockTab,SDockTab)
		.TabRole(ETabRole::NomadTab)
		.OnTabClosed_Lambda(OnTabClosed)
		[
			// Put your tab content here!
			SNew(SBox)
			.Padding(4)
			[
				SAssignNew(SceneManagerTools,SSceneManagerTools)
			]
		];

	 LoadGameData();

	 return dockTab.ToSharedRef();

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
	PlanList.Emplace(planName, planData);
}


void FSceneManagerModule::AddMatGroupData(TSharedPtr<FMaterialGroupInfo> groupInfo)
{
	if (PlanList.Contains(currentPlan))
	{
		groupInfo->Parent = currentPlan;
		
		FMaterialGroupInfo groupData;
		groupData.GroupName = groupInfo->GroupName;
		groupData.Parent = currentPlan;
		PlanList[groupInfo->Parent].GroupList.Emplace(groupData.GroupName, groupData);
	}
}

void FSceneManagerModule::AddMatInstanceData(TSharedPtr<FMaterialInfo> matInfo)
{

	if (PlanList.Contains(matInfo->ParentPlan))
	{
		if (PlanList[matInfo->ParentPlan].GroupList.Contains(matInfo->ParentGroup))
		{
			FMaterialInfo matData;
			matData.MatPath = matInfo->MatPath;
			matData.ParentGroup = matInfo->ParentGroup;
			matData.ParentPlan = matInfo->ParentPlan;
			matData.ScalarParams = matInfo->ScalarParams;
			matData.VectorParams = matInfo->VectorParams;
			PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Emplace(matInfo->MatPath, matData);
		}
	}
}

void FSceneManagerModule::SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo)
{
	FString plan = matInfo->ParentPlan;
	FString group = matInfo->ParentGroup;
	FString path = matInfo->MatPath;
	if (PlanList.Contains(plan))
	{
		if (PlanList[plan].GroupList.Contains(group))
		{
			if (PlanList[plan].GroupList[group].MatList.Contains(path))
			{
				PlanList[plan].GroupList[group].MatList[path] = *matInfo;
			}
		}
	}
}

void FSceneManagerModule::DeleteMatGroup(TSharedPtr<FMaterialGroupInfo> matInfo)
{
	FString plan = matInfo->Parent;
	FString group = matInfo->GroupName;
	if (PlanList.Contains(plan))
	{
		if (PlanList[plan].GroupList.Contains(group))
		{
			PlanList[plan].GroupList.Remove(group);

		}
	}

	/*
	重新刷新
	*/

	if (PlanList.Contains(currentPlan))
	{
		SceneManagerTools->MatGroupItems.Empty();
		SceneManagerTools->ListView->RequestListRefresh();

		for (TPair<FString, FMaterialGroupInfo> groupIt : PlanList[currentPlan].GroupList)
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
	if (PlanList.Contains(plan))
	{
		if (PlanList[plan].GroupList.Contains(group))
		{
			if (PlanList[plan].GroupList[group].MatList.Contains(path))
			{
				PlanList[plan].GroupList[group].MatList.Remove(path);
			}
		}
	}

	/*
	在切换材质球时只需要清空数据
	在删除材质球时需要调下面的方法
	*/

	
	if (PlanList.Contains(currentPlan))
	{
		SceneManagerTools->MatGroupItems.Empty();
		SceneManagerTools->ListView->RequestListRefresh();

		for (TPair<FString, FMaterialGroupInfo> groupIt : PlanList[currentPlan].GroupList)
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
	FString plan = matInfo->ParentPlan;
	FString group = matInfo->ParentGroup;
	FString path = matInfo->MatPath;
	if (PlanList.Contains(plan))
	{
		if (PlanList[plan].GroupList.Contains(group))
		{
			if (PlanList[plan].GroupList[group].MatList.Contains(originPath))
			{
				PlanList[plan].GroupList[group].MatList.Remove(originPath);
			}
		}
	}

	AddMatInstanceData(matInfo);
}


void FSceneManagerModule::SaveGameData()
{
	if (saveGame)
	{
		saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
		saveGame->PlanList = PlanList;
		saveGame->DefaultPlan = currentPlan;
		UE_LOG(LogTemp,Warning,TEXT("SaveGameData PlanList %d"), saveGame->PlanList.Num());
		UGameplayStatics::SaveGameToSlot(saveGame, TEXT("TestSlot"), 0);
	}
}

void FSceneManagerModule::LoadGameData(const FString loadPlanName)
{
	saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
	if (saveGame == nullptr)
	{
		saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadGameData %s"), *loadPlanName);
		PlanList = saveGame->PlanList;
		currentPlan = saveGame->DefaultPlan;
		RefreshPlanList(currentPlan);
	}

}

void FSceneManagerModule::RefreshPlanList(const FString planName)
{
	currentPlan = planName;

	if (!PlanList.Contains(planName))
	{
		AddPlanData(planName);
	}

	SceneManagerTools->ClearMaterialGroup();
	if (PlanList.Contains(planName))
	{
		for (TPair<FString, FMaterialGroupInfo> group : PlanList[planName].GroupList)
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