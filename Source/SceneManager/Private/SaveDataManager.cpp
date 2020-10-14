


#include "SaveDataManager.h"
#include "Kismet/GameplayStatics.h"


SaveDataManager::SaveDataManager()
{

}

SaveDataManager* SaveDataManager::instance = new SaveDataManager();

SaveDataManager* SaveDataManager::Get() {
	return instance;
}

void SaveDataManager::LoadData()
{
	saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
	if (saveGame == nullptr)
	{
		saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
	}
	saveGame->AddToRoot();

	pluginSaveSettings = NewObject<UPluginSaveSettings>();
	pluginSaveSettings->AddToRoot();
	pluginSaveSettings->MatGroupNameList = saveGame->MatGroupNameList;
	pluginSaveSettings->PlayerLightMPC = saveGame->PlayerLightMPC;
	pluginSaveSettings->SceneLightMPC = saveGame->SceneLightMPC;
	pluginSaveSettings->WindMPC = saveGame->WindMPC;


	PostProcessSetting = NewObject<ULevelPostProcessSettings>();
	PostProcessSetting->AddToRoot();
	PostProcessSetting->Setting = saveGame->PostProcessSetting;
}

void SaveDataManager::SaveData()
{
	saveGame->MatGroupNameList = pluginSaveSettings->MatGroupNameList;
	saveGame->PlayerLightMPC = pluginSaveSettings->PlayerLightMPC;
	saveGame->SceneLightMPC = pluginSaveSettings->SceneLightMPC;
	saveGame->WindMPC = pluginSaveSettings->WindMPC;
	saveGame->PostProcessSetting = PostProcessSetting->Setting;
	UGameplayStatics::SaveGameToSlot(saveGame, TEXT("TestSlot"), 0);
}



void SaveDataManager::AddMatInstanceData(TSharedPtr<FMaterialInfo> matInfo)
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

			if (!saveGame->PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Contains(matInfo->MatPath))
				saveGame->PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Emplace(matInfo->MatPath, matData);
		}
	}
}

void SaveDataManager::SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo)
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

void SaveDataManager::DeleteMatInstanceData(TSharedPtr<FMaterialInfo> matInfo)
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
}

void SaveDataManager::ReplaceMatInstanceData(TSharedPtr<FMaterialInfo> matInfo, FString originPath)
{
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
}


bool SaveDataManager::IsMaterialPlanExisted(TSharedPtr<FMaterialInfo> matInfo)
{
	return SaveDataManager::Get()->saveGame->PlanList.Contains(matInfo->ParentPlan);
}

bool SaveDataManager::IsMaterialGroupExisted(TSharedPtr<FMaterialInfo> matInfo)
{
	if (IsMaterialPlanExisted(matInfo))
		return SaveDataManager::Get()->saveGame->PlanList[matInfo->ParentPlan].GroupList.Contains(matInfo->ParentGroup);
	else
		return false;
}

bool SaveDataManager::IsMaterialExisted(TSharedPtr<FMaterialInfo> matInfo)
{
	return false;
}


FMaterialPlanInfo* SaveDataManager::GetSelectedPlan()
{
	if (saveGame->PlanList.Contains(saveGame->DefaultPlan))
		return &saveGame->PlanList[saveGame->DefaultPlan];
	else
		return nullptr;
}

FMaterialGroupInfo* SaveDataManager::GetGroup(TSharedPtr<FMaterialInfo> matInfo)
{
	if (IsMaterialGroupExisted(matInfo))
		return &saveGame->PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup];
	else
		return nullptr;
}

void SaveDataManager::CopyPlanData(const FMaterialPlanInfo* fromPlan, FMaterialPlanInfo* toPlan)
{
	for (TPair<FString, FMaterialGroupInfo> GroupInfoPair: fromPlan->GroupList)
	{
		FMaterialGroupInfo FromGroupInfo = GroupInfoPair.Value;
		FMaterialGroupInfo ToGroupInfo;
		for (TPair<FString, FMaterialInfo> MatInfoPari: FromGroupInfo.MatList)
		{
			FMaterialInfo FromMatInfo = MatInfoPari.Value;
			FMaterialInfo ToMatInfo;
			for (TPair<FString, float> ScalarInfoPair : FromMatInfo.ScalarParams)
			{
				ToMatInfo.ScalarParams.Emplace(ScalarInfoPair.Key, ScalarInfoPair.Value);
			}
			for (TPair<FString, FLinearColor> VectorInfoPair : FromMatInfo.VectorParams)
			{
				ToMatInfo.VectorParams.Emplace(VectorInfoPair.Key, VectorInfoPair.Value);
			}
			ToMatInfo.MatPath = FromMatInfo.MatPath;
			ToMatInfo.ParentGroup = FromMatInfo.ParentGroup;
			ToMatInfo.ParentPlan = toPlan->Name;
			ToGroupInfo.MatList.Emplace(ToMatInfo.MatPath, ToMatInfo);
		}
		ToGroupInfo.GroupName = FromGroupInfo.GroupName;
		ToGroupInfo.Parent = toPlan->Name;
		toPlan->GroupList.Emplace(ToGroupInfo.GroupName, ToGroupInfo);
	}
}
