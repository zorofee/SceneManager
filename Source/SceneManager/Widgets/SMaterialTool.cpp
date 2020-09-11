// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialTool.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Materials/MaterialInstance.h"


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
		TArray<FMaterialParameterInfo> ScalarParameterInfo;
		TArray<FGuid> ScalarParameterGuids;
		FString GroupName;
		
		//Get scalar parameters name and value
		Material->GetAllScalarParameterInfo(ScalarParameterInfo, ScalarParameterGuids);
		for (size_t i = 0; i < ScalarParameterInfo.Num(); i++)
		{
			FName groupName;
			Material->GetGroupName(ScalarParameterInfo[i], groupName);
			UE_LOG(LogTemp, Warning, TEXT("Group name is %s , %s"), *groupName.ToString(), *ScalarParameterInfo[i].Name.ToString());
			
			GroupName = groupName.ToString();
			if (GroupName.Equals(TEXT("LevelMaterial")))
			{
				FMaterialInfo* info = new FMaterialInfo();
				info->mat = Material;
				info->matName = ScalarParameterInfo[i].Name.ToString();
				Material->GetScalarParameterValue(ScalarParameterInfo[i], info->scalarValue);
				ScalarParamsPair.Emplace(info->matName, info);
			}
		}

		//Get vector parameters name and value



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
			.Text(FText::FromString(info->matName))
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





	//param2 color
	/*+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Param2")))
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(5)
		]


	+SHorizontalBox::Slot()
		[
			SAssignNew(ColorImage, SImage)
			.ColorAndOpacity(FSlateColor(CachedColor))
			.OnMouseButtonDown(this, &SMaterialTool::OnClickColorBlock)
		]*/
}



void SMaterialTool::OnScalarValueChanged(float value,FMaterialInfo* info)
{
	UE_LOG(LogTemp, Warning, TEXT("%s %s ValueChanged %f"), *info->matName, *info->mat->GetName(), info->scalarValue);
	DelegateManager::Get()->OnMatScalarValueChanged.Broadcast(info->matName,info->scalarValue);

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
