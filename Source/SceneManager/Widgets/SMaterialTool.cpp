// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialTool.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Materials/MaterialInstance.h"


#include "Kismet/GameplayStatics.h"
#include "SceneManagerSaveGame.h"


void SMaterialTool::Construct(const FArguments& InArgs)
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> LevelMatView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	lms = NewObject<ULevelMaterialSettings>();
	LevelMatView->SetObject(lms);
	LevelMatView->OnFinishedChangingProperties().AddRaw(this, &SMaterialTool::OnFinishedChangingProperties);

	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		.AutoHeight()
		[
			LevelMatView
		]
		
		+SVerticalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this,&SMaterialTool::TestSaveData)
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(ParamContainer, SVerticalBox)
		]
	];

}

void SMaterialTool::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	AnalysisMaterialParams();

	for (TPair<FString, FMaterialInfo*> item : VectorParamsPair)
	{
		ParamContainer->AddSlot()
			.AutoHeight()
			[
				GetVectorParamSlot(item.Value)
			];
	}

	for (TPair<FString,FMaterialInfo*> item : ScalarParamsPair)
	{
		ParamContainer->AddSlot()
		.AutoHeight()
		[
			GetScalarParamSlot(item.Value)
		];
	}

}

void SMaterialTool::AnalysisMaterialParams()
{
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

	}
}


TSharedRef<SHorizontalBox> SMaterialTool::GetScalarParamSlot(FMaterialInfo* info)
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
			.OnValueChanged(this, &SMaterialTool::OnScalarValueChanged, info)
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


TSharedRef<SHorizontalBox> SMaterialTool::GetVectorParamSlot(FMaterialInfo* info)
{
	return
		SNew(SHorizontalBox)

	+SHorizontalBox::Slot()
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
		.OnMouseButtonDown(this, &SMaterialTool::OnClickColorBlock)
		];
}

void SMaterialTool::OnScalarValueChanged(float value,FMaterialInfo* info)
{
	UE_LOG(LogTemp, Warning, TEXT("%s %s ValueChanged %f"), *info->paramName, *info->mat->GetName(), info->scalarValue);
	DelegateManager::Get()->OnMatScalarValueChanged.Broadcast(info->mat,info->paramName,info->scalarValue);

}


FReply SMaterialTool::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = true;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SMaterialTool::OnSetColorFromColorPicker);
		PickerArgs.InitialColorOverride = CachedColor;
		PickerArgs.ParentWidget = ColorImage;
	}

	//FVariantColorNodeImpl::GetCommonColorFromPropertyValues(PickerArgs.InitialColorOverride, PropertyValues);

	OpenColorPicker(PickerArgs);
	return FReply::Handled();

}


void SMaterialTool::OnSetColorFromColorPicker(FLinearColor NewColor)
{
	UE_LOG(LogTemp,Warning,TEXT("OnSetColorFromColorPicker %s"),*NewColor.ToString());
	CachedColor = NewColor;
	ColorImage->SetColorAndOpacity(CachedColor);
}




FReply SMaterialTool::TestSaveData()
{
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