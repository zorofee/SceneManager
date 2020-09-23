// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialIntermediate.h"
#include "DelegateManager.h"
#include "Kismet/GameplayStatics.h"

void MaterialIntermediate::AddEventListener()
{

	if (!DelegateManager::Get()->SaveGameData.IsBound())
		DelegateManager::Get()->SaveGameData.AddRaw(this, &MaterialIntermediate::SaveGameData);


	if (!DelegateManager::Get()->LoadGameData.IsBound())
		DelegateManager::Get()->LoadGameData.AddRaw(this, &MaterialIntermediate::LoadGameData);


	if (!DelegateManager::Get()->AddSceneMatPlan.IsBound())
		DelegateManager::Get()->AddSceneMatPlan.AddRaw(this, &MaterialIntermediate::AddMatPlan);

	if (!DelegateManager::Get()->AddSceneMatGroup.IsBound())
		DelegateManager::Get()->AddSceneMatGroup.AddRaw(this, &MaterialIntermediate::AddMatGroup);

	if (!DelegateManager::Get()->AddSceneMatInstance.IsBound())
		DelegateManager::Get()->AddSceneMatInstance.AddRaw(this, &MaterialIntermediate::AddMatInstance);

	if (!DelegateManager::Get()->OnMatParamChanged.IsBound())
		DelegateManager::Get()->OnMatParamChanged.AddRaw(this, &MaterialIntermediate::SetMatScalarParam);



	if (!DelegateManager::Get()->RefreshPlanList.IsBound())
		DelegateManager::Get()->RefreshPlanList.AddRaw(this, &MaterialIntermediate::SelectMatPlan);

	if (!DelegateManager::Get()->DeleteSceneMatPlan.IsBound())
		DelegateManager::Get()->DeleteSceneMatPlan.AddRaw(this, &MaterialIntermediate::DeleteMatPlan);

	if (!DelegateManager::Get()->DeleteSceneMatGroup.IsBound())
		DelegateManager::Get()->DeleteSceneMatGroup.AddRaw(this, &MaterialIntermediate::DeleteMatGroup);

	if (!DelegateManager::Get()->DeleteSceneMatInstance.IsBound())
		DelegateManager::Get()->DeleteSceneMatInstance.AddRaw(this, &MaterialIntermediate::DeleteMatInstance);

	if (!DelegateManager::Get()->ReplaceSceneMatInstance.IsBound())
		DelegateManager::Get()->ReplaceSceneMatInstance.AddRaw(this, &MaterialIntermediate::ReplaceMatInstance);

	if (!DelegateManager::Get()->SelectMaterialInstance.IsBound())
		DelegateManager::Get()->SelectMaterialInstance.AddRaw(this, &MaterialIntermediate::SelectMaterialInstance);

}



void MaterialIntermediate::SaveGameData()
{
	if (saveGame)
	{
		/*
			Save Material Plan
		*/
		saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
		saveGame->PlanList = PlanList;
		saveGame->DefaultPlan = currentPlan;
		UE_LOG(LogTemp, Warning, TEXT("SaveGameData PlanList %d"), saveGame->PlanList.Num());



		/*
			Save Game Data
		*/
		UGameplayStatics::SaveGameToSlot(saveGame, TEXT("TestSlot"), 0);
	}
}

void MaterialIntermediate::LoadGameData(const FString loadPlanName)
{
	saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
	if (saveGame == nullptr)
	{
		saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
	}
	else
	{


		/*
			Refresh Material Plan
		*/
		UE_LOG(LogTemp, Warning, TEXT("LoadGameData %s"), *loadPlanName);
		PlanList = saveGame->PlanList;
		SelectMatPlan(saveGame->DefaultPlan);

		//refresh combobox when first load savedata
		TArray<FString> planNameArr;
		PlanList.GenerateKeyArray(planNameArr);
		SceneManagerTools->SceneMaterialManager->ResetPlanComboBox(planNameArr);
	}

	/*
	Refresh PostProcess
	*/
	SceneManagerTools->PostProcessManager->RefreshContentList();
}



void MaterialIntermediate::AddMatPlan(FString planName)
{
	if (!PlanList.Contains(planName))
	{
		//1.添加数据
		FMaterialPlanInfo planData;
		planData.Name = planName;
		PlanList.Emplace(planName, planData);

		//2.刷新Combobox列表
		TArray<FString> planNameArr;
		PlanList.GenerateKeyArray(planNameArr);
		SceneManagerTools->SceneMaterialManager->ResetPlanComboBox(planNameArr);

		//3.刷新底下材质数据为当前选中的方案
		SelectMatPlan(planName);
	}
}


void MaterialIntermediate::DeleteMatPlan(FString planName)
{
	if (PlanList.Contains(planName))
	{
		//UE_LOG(LogTemp,Warning,TEXT("DeleteMatPlan"));
		//1.刪除数据
		PlanList.Remove(planName);

		//2.刷新Combobox列表
		TArray<FString> planNameArr;
		PlanList.GenerateKeyArray(planNameArr);
		SceneManagerTools->SceneMaterialManager->ResetPlanComboBox(planNameArr);

		//3.刷新底下材质数据为默认第一个
		if (planNameArr.Num() > 0)
		{
			SelectMatPlan(planNameArr[0]);
		}
	}

}


void MaterialIntermediate::SelectMatPlan(const FString planName)
{
	UE_LOG(LogTemp, Warning, TEXT("SelectMatPlan 000 %s"), *planName);
	currentPlan = planName;
	//1.刷新Combobox列表的默认选项
	SceneManagerTools->SceneMaterialManager->SetSelectedPlanName(currentPlan);

	//2.更新下面的材质
	SceneManagerTools->SceneMaterialManager->ClearMaterialGroup();
	if (PlanList.Contains(planName))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectMatPlan %s"), *planName);
		for (TPair<FString, FMaterialGroupInfo> group : PlanList[planName].GroupList)
		{
			if (SceneManagerTools->SceneMaterialManager)
			{
				SceneManagerTools->SceneMaterialManager->AddMaterialGroup(group.Value);
			}
		}
	}
}




void MaterialIntermediate::AddMatGroup(FString groupName)
{
	if (PlanList.Contains(currentPlan))
	{
		if (!PlanList[currentPlan].GroupList.Contains(groupName))
		{
			//1.添加数据
			FMaterialGroupInfo groupData;
			groupData.GroupName = groupName;
			groupData.Parent = currentPlan;
			PlanList[currentPlan].GroupList.Emplace(groupData.GroupName, groupData);

			//2.刷新底下的材质
			SceneManagerTools->SceneMaterialManager->AddMaterialGroup(groupData);
		}
	}
}


void MaterialIntermediate::DeleteMatGroup(TSharedPtr<FMaterialGroupInfo> matInfo)
{
	FString plan = matInfo->Parent;
	FString group = matInfo->GroupName;
	if (PlanList.Contains(plan))
	{
		if (PlanList[plan].GroupList.Contains(group))
		{
			//1.移除数据
			PlanList[plan].GroupList.Remove(group);

			//2.刷新UI
			SceneManagerTools->SceneMaterialManager->DeleteMaterialGroup(matInfo);

		}
	}
}



void MaterialIntermediate::AddMatInstance(TSharedPtr<FMaterialInfo> matInfo)
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

			if (!PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Contains(matInfo->MatPath))
				PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Emplace(matInfo->MatPath, matData);
		}
	}
}

void MaterialIntermediate::SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo)
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



void MaterialIntermediate::DeleteMatInstance(TSharedPtr<FMaterialInfo> matInfo)
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
		SceneManagerTools->SceneMaterialManager->MatGroupItems.Empty();
		SceneManagerTools->SceneMaterialManager->MatGroupListView->RequestListRefresh();

		for (TPair<FString, FMaterialGroupInfo> groupIt : PlanList[currentPlan].GroupList)
		{
			if (SceneManagerTools->SceneMaterialManager)
			{
				SceneManagerTools->SceneMaterialManager->AddMaterialGroup(groupIt.Value);
			}
		}
	}

}

void MaterialIntermediate::ReplaceMatInstance(TSharedPtr<FMaterialInfo> matInfo, FString originPath)
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

	AddMatInstance(matInfo);
}


void MaterialIntermediate::SelectMaterialInstance(FString newPath, TSharedPtr<FMaterialInfo> matInfo, SMaterialGroupItemWidget* widget)
{
	if (PlanList.Contains(matInfo->ParentPlan))
	{
		if (PlanList[matInfo->ParentPlan].GroupList.Contains(matInfo->ParentGroup))
		{
			if (!PlanList[matInfo->ParentPlan].GroupList[matInfo->ParentGroup].MatList.Contains(newPath))
			{
				widget->ChangeSelectedMatInstance();
			}
			else
			{
				widget->SelectedRepeatedMatInstance();
			}
		}
	}
}
