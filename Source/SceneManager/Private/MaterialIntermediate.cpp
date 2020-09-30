// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialIntermediate.h"
#include "DelegateManager.h"
#include "Kismet/GameplayStatics.h"
#include "SaveDataManager.h"

void MaterialIntermediate::AddEventListener()
{

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

	if (!DelegateManager::Get()->PopupMsgDialog.IsBound())
		DelegateManager::Get()->PopupMsgDialog.AddRaw(this, &MaterialIntermediate::PopupMsgDialog);

	if (!DelegateManager::Get()->AddPostProcessPlan.IsBound())
		DelegateManager::Get()->AddPostProcessPlan.AddRaw(this, &MaterialIntermediate::AddPostProcessPlan);

}


void MaterialIntermediate::LoadSceneManagerTools()
{
	SelectMatPlan(SaveDataManager::Get()->saveGame->DefaultPlan);
	//refresh combobox when first load savedata
	TArray<FString> planNameArr;
	SaveDataManager::Get()->saveGame->PlanList.GenerateKeyArray(planNameArr);
	SceneManagerTools->SceneMaterialManager->ResetPlanComboBox(planNameArr);
	SceneManagerTools->PostProcessManager->RefreshContentList();
}


void MaterialIntermediate::AddPostProcessPlan(const FString planName)
{
	if (!SaveDataManager::Get()->saveGame->PostProcessList.Contains(planName))
	{
		UE_LOG(LogTemp,Warning,TEXT("AddPostProcessPlan %s"),*planName);
		FPostProcessSettings planData;
		SaveDataManager::Get()->saveGame->PostProcessList.Emplace(planName, planData);
		SceneManagerTools->PostProcessManager->GetPostProcessParams(planData);
	}
}

void MaterialIntermediate::AddMatPlan(FString planName)
{
	if (!SaveDataManager::Get()->saveGame->PlanList.Contains(planName))
	{
		//1.添加数据
		FMaterialPlanInfo planData;
		planData.Name = planName;
		if (SaveDataManager::Get()->bAllowCopySelectedPlan)
		{
			SaveDataManager::Get()->CopyPlanData(SaveDataManager::Get()->GetSelectedPlan(), &planData);
		}
		SaveDataManager::Get()->saveGame->PlanList.Emplace(planName, planData);

		//2.刷新Combobox列表
		TArray<FString> planNameArr;
		SaveDataManager::Get()->saveGame->PlanList.GenerateKeyArray(planNameArr);
		SceneManagerTools->SceneMaterialManager->ResetPlanComboBox(planNameArr);

		//3.刷新底下材质数据为当前选中的方案
		SelectMatPlan(planName);
	}
	else
	{
		FString Warning = FString::Printf(TEXT("同名方案已经存在了哦"));
		DelegateManager::Get()->PopupMsgDialog.Broadcast(Warning);
	}
}


void MaterialIntermediate::DeleteMatPlan(FString planName)
{
	if (SaveDataManager::Get()->saveGame->PlanList.Contains(planName))
	{
		//UE_LOG(LogTemp,Warning,TEXT("DeleteMatPlan"));
		//1.刪除数据
		SaveDataManager::Get()->saveGame->PlanList.Remove(planName);

		//2.刷新Combobox列表
		TArray<FString> planNameArr;
		SaveDataManager::Get()->saveGame->PlanList.GenerateKeyArray(planNameArr);
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
	SaveDataManager::Get()->saveGame->DefaultPlan = planName;
	//1.刷新Combobox列表的默认选项
	SceneManagerTools->SceneMaterialManager->SetSelectedPlanName(SaveDataManager::Get()->saveGame->DefaultPlan);

	//2.更新下面的材质
	SceneManagerTools->SceneMaterialManager->ClearMaterialGroup();
	if (SaveDataManager::Get()->saveGame->PlanList.Contains(planName))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectMatPlan %s"), *planName);
		for (TPair<FString, FMaterialGroupInfo> group : SaveDataManager::Get()->saveGame->PlanList[planName].GroupList)
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
	if (SaveDataManager::Get()->saveGame->PlanList.Contains(SaveDataManager::Get()->saveGame->DefaultPlan))
	{
		if (!SaveDataManager::Get()->GetSelectedPlan()->GroupList.Contains(groupName))
		{
			//1.添加数据
			FMaterialGroupInfo groupData;
			groupData.GroupName = groupName;
			groupData.Parent = SaveDataManager::Get()->saveGame->DefaultPlan;
			SaveDataManager::Get()->GetSelectedPlan()->GroupList.Emplace(groupData.GroupName, groupData);

			//2.刷新底下的材质
			SceneManagerTools->SceneMaterialManager->AddMaterialGroup(groupData);
		}
		else
		{
			FString Warning = FString::Printf(TEXT("同名分组已经存在了哦"));
			DelegateManager::Get()->PopupMsgDialog.Broadcast(Warning);
		}
	}
}


void MaterialIntermediate::DeleteMatGroup(TSharedPtr<FMaterialGroupInfo> matInfo)
{
	FString plan = matInfo->Parent;
	FString group = matInfo->GroupName;
	if (SaveDataManager::Get()->saveGame->PlanList.Contains(plan))
	{
		if (SaveDataManager::Get()->saveGame->PlanList[plan].GroupList.Contains(group))
		{
			//1.移除数据
			SaveDataManager::Get()->saveGame->PlanList[plan].GroupList.Remove(group);

			//2.刷新UI
			SceneManagerTools->SceneMaterialManager->DeleteMaterialGroup(matInfo);

		}
	}
}



void MaterialIntermediate::AddMatInstance(TSharedPtr<FMaterialInfo> matInfo)
{
	SaveDataManager::Get()->AddMatInstanceData(matInfo);
}

void MaterialIntermediate::SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo)
{
	SaveDataManager::Get()->SetMatScalarParam(matInfo);
}



void MaterialIntermediate::DeleteMatInstance(TSharedPtr<FMaterialInfo> matInfo)
{
	SaveDataManager::Get()->DeleteMatInstanceData(matInfo);

	/*
	在切换材质球时只需要清空数据
	在删除材质球时需要调下面的方法
	*/
	if (SaveDataManager::Get()->GetSelectedPlan() != nullptr)
	{
		SceneManagerTools->SceneMaterialManager->MatGroupItems.Empty();
		SceneManagerTools->SceneMaterialManager->MatGroupListView->RequestListRefresh();

		for (TPair<FString, FMaterialGroupInfo> groupIt : SaveDataManager::Get()->GetSelectedPlan()->GroupList)
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
	SaveDataManager::Get()->ReplaceMatInstanceData(matInfo, originPath);
	AddMatInstance(matInfo);
}


void MaterialIntermediate::SelectMaterialInstance(FString newPath, TSharedPtr<FMaterialInfo> matInfo, SMaterialGroupItemWidget* widget)
{
	if (newPath.ToLower() == TEXT("none") && matInfo->MatPath == TEXT(""))
	{
		//添加材质后,想删除当前的空材质
		if (SaveDataManager::Get()->saveGame->PlanList.Contains(matInfo->ParentPlan))
		{
			SceneManagerTools->SceneMaterialManager->MatGroupItems.Empty();
			SceneManagerTools->SceneMaterialManager->MatGroupListView->RequestListRefresh();

			for (TPair<FString, FMaterialGroupInfo> groupIt : SaveDataManager::Get()->saveGame->PlanList[matInfo->ParentPlan].GroupList)
			{
				if (SceneManagerTools->SceneMaterialManager)
				{
					SceneManagerTools->SceneMaterialManager->AddMaterialGroup(groupIt.Value);
				}
			}
		}
		return;
	}



	if (SaveDataManager::Get()->IsMaterialPlanExisted(matInfo))
	{
		if (SaveDataManager::Get()->IsMaterialGroupExisted(matInfo))
		{
			if (!SaveDataManager::Get()->GetGroup(matInfo)->MatList.Contains(newPath))
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


void MaterialIntermediate::PopupMsgDialog(const FString content)
{
	FText Title = FText::FromString("Warning");
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(*content), &Title);
}

