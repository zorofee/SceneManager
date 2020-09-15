// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupItemWidget.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Materials/MaterialInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SceneManagerSaveGame.h"



void SMaterialGroupItemEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SMaterialGroupItemWidget, InItem)
		]
	];
}



/*
材质控件有两种创建方式:
1.新增,此时参数全为空;
2.根据保存数据加载,此时参数按照加载的材质球上的参数创建。
*/
void SMaterialGroupItemWidget::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem)
{
	m_GroupItemInfo.ParentPlan = InItem.Get()->ParentPlan;
	m_GroupItemInfo.ParentGroup = InItem.Get()->ParentGroup;


	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> LevelMatView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	lms = NewObject<ULevelMaterialSettings>();
	LevelMatView->SetObject(lms);
	LevelMatView->OnFinishedChangingProperties().AddRaw(this, &SMaterialGroupItemWidget::OnFinishedChangingProperties);

	ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			LevelMatView
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ParamContainer, SVerticalBox)
		]
		];
}


void SMaterialGroupItemWidget::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	AnalysisMaterialParams();
}



TSharedRef<SHorizontalBox> SMaterialGroupItemWidget::GetScalarParamSlot(FMaterialInfo* info)
{
	return
		SNew(SHorizontalBox)

		//param1
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		[
			SNew(STextBlock)
			.Text(FText::FromString(info->paramName))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(5)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpinBox<float>)
			.OnValueChanged(this, &SMaterialGroupItemWidget::OnScalarValueChanged, info)
		.MinValue(-100)
		.MaxValue(100)
		.Value(info->scalarValue)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(20)
		];


}


TSharedRef<SHorizontalBox> SMaterialGroupItemWidget::GetVectorParamSlot(FMaterialInfo* info)
{
	return
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		[
			SNew(STextBlock)
			.Text(FText::FromString(info->paramName))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(5)
		]


	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SAssignNew(ColorImage, SImage)
			.ColorAndOpacity(FSlateColor(CachedColor))
		.OnMouseButtonDown(this, &SMaterialGroupItemWidget::OnClickColorBlock)
		];
}

void SMaterialGroupItemWidget::OnScalarValueChanged(float value, FMaterialInfo* info)
{
	UE_LOG(LogTemp, Warning, TEXT("%s %s ValueChanged %f"), *info->paramName, *info->mat->GetName(), info->scalarValue);
	DelegateManager::Get()->OnMatScalarValueChanged.Broadcast(info->mat, info->paramName, info->scalarValue);

}


FReply SMaterialGroupItemWidget::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = true;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SMaterialGroupItemWidget::OnSetColorFromColorPicker);
		PickerArgs.InitialColorOverride = CachedColor;
		PickerArgs.ParentWidget = ColorImage;
	}

	//FVariantColorNodeImpl::GetCommonColorFromPropertyValues(PickerArgs.InitialColorOverride, PropertyValues);

	OpenColorPicker(PickerArgs);
	return FReply::Handled();

}


void SMaterialGroupItemWidget::OnSetColorFromColorPicker(FLinearColor NewColor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSetColorFromColorPicker %s"), *NewColor.ToString());
	CachedColor = NewColor;
	ColorImage->SetColorAndOpacity(CachedColor);
}

void SMaterialGroupItemWidget::LoadCurrentMaterial(const FString path)
{
	UE_LOG(LogTemp, Warning, TEXT("Load current material %s"), *path);
	//UObject* loadObj = StaticLoadObject(UMaterialInstance::StaticClass(), NULL, TEXT("/Game/Grass_Ins.Grass_Ins"));
	UObject* loadMat = StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *path);
	if (loadMat != nullptr)
	{
		lms->material = Cast<UMaterialInstance>(loadMat);
	}
	AnalysisMaterialParams();
}


void SMaterialGroupItemWidget::AnalysisMaterialParams()
{

	ParamContainer.Get()->ClearChildren();
	VectorParamsPair.Empty();
	ScalarParamsPair.Empty();

	UMaterialInstance* Material = lms->material;
	if (Material)
	{
		TArray<FMaterialParameterInfo> ParameterInfo;
		TArray<FGuid> ParameterGuids;
		FString GroupName;

		//Get scalar parameters name and value
		Material->GetAllScalarParameterInfo(ParameterInfo, ParameterGuids);
		for (size_t i = 0; i < ParameterInfo.Num(); i++)
		{
			FName groupName;
			Material->GetGroupName(ParameterInfo[i], groupName);
			UE_LOG(LogTemp, Warning, TEXT("Group name is %s , %s"), *groupName.ToString(), *ParameterInfo[i].Name.ToString());

			GroupName = groupName.ToString();
			if (GroupName.Equals(TEXT("LevelMaterial")))
			{
				FMaterialInfo* info = new FMaterialInfo();
				info->mat = Material;
				info->paramName = ParameterInfo[i].Name.ToString();
				Material->GetScalarParameterValue(ParameterInfo[i], info->scalarValue);
				ScalarParamsPair.Emplace(info->paramName, info);
			}
		}

		//Get vector parameters name and value
		ParameterInfo.Empty();
		ParameterGuids.Empty();
		Material->GetAllVectorParameterInfo(ParameterInfo, ParameterGuids);
		for (size_t i = 0; i < ParameterInfo.Num(); i++)
		{
			FName groupName;
			Material->GetGroupName(ParameterInfo[i], groupName);
			UE_LOG(LogTemp, Warning, TEXT("Group name is %s , %s"), *groupName.ToString(), *ParameterInfo[i].Name.ToString());

			GroupName = groupName.ToString();
			if (GroupName.Equals(TEXT("LevelMaterial")))
			{
				FMaterialInfo* info = new FMaterialInfo();
				info->mat = Material;
				info->paramName = ParameterInfo[i].Name.ToString();
				Material->GetVectorParameterValue(ParameterInfo[i], info->vectorValue);
				VectorParamsPair.Emplace(info->paramName, info);
			}
		}

		AddParamsToSlot();


		m_GroupItemInfo.MatPath = Material->GetPathName();
	}

	UE_LOG(LogTemp, Warning, TEXT("Plan is %s , Group name is %s, Mat path is %s"), *m_GroupItemInfo.ParentPlan, *m_GroupItemInfo.ParentGroup, *m_GroupItemInfo.MatPath);
}

void SMaterialGroupItemWidget::AddParamsToSlot()
{
	for (TPair<FString, FMaterialInfo*> item : VectorParamsPair)
	{
		ParamContainer->AddSlot()
			.AutoHeight()
			[
				GetVectorParamSlot(item.Value)
			];
	}

	for (TPair<FString, FMaterialInfo*> item : ScalarParamsPair)
	{
		ParamContainer->AddSlot()
			.AutoHeight()
			[
				GetScalarParamSlot(item.Value)
			];
	}
}



FReply SMaterialGroupItemWidget::TestSaveData()
{
	USceneManagerSaveGame* saveGame1 = Cast<USceneManagerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TestSlot"), 0));
	FString path = saveGame1->PlanList[0].GroupList[0].MatList[0].MatPath;
	LoadCurrentMaterial(path);

	return FReply::Handled();


	USceneManagerSaveGame* saveGame = Cast<USceneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USceneManagerSaveGame::StaticClass()));
	saveGame->TestName = FString::Printf(TEXT("testtttt"));


	FMatData matData;
	matData.Name = FString::Printf(TEXT("TestMatData2222"));
	matData.MatPath = lms->material->GetPathName();
	matData.ScalarParams.Emplace(1.34);
	matData.VectorParams.Emplace(FVector(1, 2, 3));

	FGroupData groupData;
	groupData.Name = FString::Printf(TEXT("TestGroupData2222"));
	groupData.MatList.Emplace(matData);

	FPlanData planData;
	planData.Name = FString::Printf(TEXT("TestPlanData2222"));
	planData.GroupList.Emplace(groupData);

	saveGame->PlanList.Emplace(planData);

	UGameplayStatics::SaveGameToSlot(saveGame, TEXT("TestSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("TestSaveData %s  , plan data num is %d"), *saveGame->TestName, saveGame->PlanList.Num());

	return FReply::Handled();
}