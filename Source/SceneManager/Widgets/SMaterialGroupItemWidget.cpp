// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupItemWidget.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Materials/MaterialInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SceneManagerSaveGame.h"

#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"


void SMaterialGroupItemEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialInfo>& InItem)
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
void SMaterialGroupItemWidget::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialInfo>& MatInfo)
{
	SaveMaterialInfo(MatInfo);

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
			SAssignNew(ParamContainer, SUniformGridPanel)
			.SlotPadding(2)
		]

		];

	
	//如果已有材质路径则直接根据已有的数据刷新参数
	if (m_MaterialInfo->MatPath != TEXT(""))
	{
		LoadMaterialInstanceByInfo();
	}

}


void SMaterialGroupItemWidget::SaveMaterialInfo(const TSharedPtr<const FMaterialInfo>& MatInfo)
{
	m_MaterialInfo = MakeShareable(new FMaterialInfo());
	m_MaterialInfo->ParentPlan = MatInfo->ParentPlan;
	m_MaterialInfo->ParentGroup = MatInfo->ParentGroup;
	m_MaterialInfo->MatPath = MatInfo->MatPath;
	m_MaterialInfo->ScalarParams = MatInfo->ScalarParams;
	m_MaterialInfo->VectorParams = MatInfo->VectorParams;
}



void SMaterialGroupItemWidget::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	FString originPath = m_MaterialInfo->MatPath;
	AnalysisMaterialParams();

	UE_LOG(LogTemp, Warning, TEXT("O : %s, C : %s"), *originPath, *m_MaterialInfo->MatPath);

	if (originPath == TEXT(""))
	{
		//若之前材质球为空且当前选择材质球不为空，则刷新数据
		if (m_MaterialInfo->MatPath != TEXT(""))
			DelegateManager::Get()->OnMatParamChanged.Broadcast(m_MaterialInfo);
	}
	else
	{
		if (m_MaterialInfo->MatPath == originPath)
		{
			//若之前材质球不为空,当前选择材质球跟之前一样（选clear时一样），则删除
			DelegateManager::Get()->DeleteSceneMatInstance.Broadcast(m_MaterialInfo);
		}
		else
		{
			//若之前材质球和当前选择的材质球都不为空,，则替换数据
			DelegateManager::Get()->ReplaceSceneMatInstance.Broadcast(m_MaterialInfo, originPath);
		}
	}
}



TSharedRef<SHorizontalBox> SMaterialGroupItemWidget::GetScalarParamSlot(FString name , float value)
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
			.Text(FText::FromString(name))
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
		.OnValueChanged(this, &SMaterialGroupItemWidget::OnScalarValueChanged, name)
		.MinValue(-100)
		.MaxValue(100)
		.Value(value)
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(20)
		];

}


TSharedRef<SHorizontalBox> SMaterialGroupItemWidget::GetVectorParamSlot(FString name, FLinearColor value)
{
	return
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Bottom)
		[
			SNew(STextBlock)
			.Text(FText::FromString(name))
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
			.ColorAndOpacity(FSlateColor(value))
			.OnMouseButtonDown(this, &SMaterialGroupItemWidget::OnClickColorBlock, name)
		];
}

void SMaterialGroupItemWidget::OnScalarValueChanged(float value, FString name)
{
	m_MaterialInfo->ScalarParams[name] = value;
	DelegateManager::Get()->OnMatParamChanged.Broadcast(m_MaterialInfo);

	
	
	//修改场景中的动态材质实例
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Actor num is %d"), actorList.Num());
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		AStaticMeshActor* meshActor = Cast<AStaticMeshActor>(actorList[i]);
		TArray<UMaterialInterface*> mats = meshActor->GetStaticMeshComponent()->GetMaterials();
		for (size_t j = 0; j < mats.Num(); j++)
		{
			if (UMaterialInstance* DynMaterial = Cast<UMaterialInstance>(mats[j]))
			{
				if(DynMaterial == lms->material)
					UE_LOG(LogTemp,Warning,TEXT("DYN %s, LMS %s "), *DynMaterial->GetName(), *lms->material->GetName());
				
				meshActor->GetStaticMeshComponent()->SetScalarParameterValueOnMaterials(FName(name),value);

			}
		}
	}

}


FReply SMaterialGroupItemWidget::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = true;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SMaterialGroupItemWidget::OnSetColorFromColorPicker, name);
		PickerArgs.InitialColorOverride = CachedColor;
		PickerArgs.ParentWidget = ColorImage;
	}

	OpenColorPicker(PickerArgs);
	return FReply::Handled();

}


void SMaterialGroupItemWidget::OnSetColorFromColorPicker(FLinearColor NewColor,FString name)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSetColorFromColorPicker %s"), *NewColor.ToString());
	CachedColor = NewColor;
	ColorImage->SetColorAndOpacity(CachedColor);

	m_MaterialInfo->VectorParams[name] = NewColor;
	DelegateManager::Get()->OnMatParamChanged.Broadcast(m_MaterialInfo);
}

void SMaterialGroupItemWidget::LoadMaterialInstanceByInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Load current material %s"), *m_MaterialInfo->MatPath);
	UObject* loadMat = StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *m_MaterialInfo->MatPath);
	if (loadMat != nullptr)
	{
		lms->material = Cast<UMaterialInstance>(loadMat);
	}
	AddParamsToSlot();
}


void SMaterialGroupItemWidget::AnalysisMaterialParams()
{

	ParamContainer.Get()->ClearChildren();
	m_MaterialInfo->VectorParams.Empty();
	m_MaterialInfo->ScalarParams.Empty();

	UMaterialInstance* Material = lms->material;
	if (Material)
	{
		TArray<FMaterialParameterInfo> ParameterInfo;
		TArray<FGuid> ParameterGuids;
		FString GroupName;
		FString ParamName;
		float ScalarValue;
		FLinearColor VectorValue;
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
				ParamName = ParameterInfo[i].Name.ToString();
				Material->GetScalarParameterValue(ParameterInfo[i], ScalarValue);
				m_MaterialInfo->ScalarParams.Emplace(ParamName, ScalarValue);

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

			GroupName = groupName.ToString();
			if (GroupName.Equals(TEXT("LevelMaterial")))
			{
				ParamName = ParameterInfo[i].Name.ToString();
				Material->GetVectorParameterValue(ParameterInfo[i], VectorValue);
				m_MaterialInfo->VectorParams.Emplace(ParamName, VectorValue);
				UE_LOG(LogTemp, Warning, TEXT("Group name is %s , %s , %s"), *groupName.ToString(), *ParameterInfo[i].Name.ToString(), *VectorValue.ToString());
			}
		}
		AddParamsToSlot();
		m_MaterialInfo->MatPath = Material->GetPathName();
		DelegateManager::Get()->AddSceneMatInstance.Broadcast(m_MaterialInfo);
	}
}


void SMaterialGroupItemWidget::AddParamsToSlot()
{
	int i = 0 , j = 0;
	for (TPair<FString, FLinearColor> item : m_MaterialInfo->VectorParams)
	{
		ParamContainer->AddSlot(i++,j)
			//.AutoHeight()
		[
			GetVectorParamSlot(item.Key,item.Value)
		];
	}

	for (TPair<FString, float> item : m_MaterialInfo->ScalarParams)
	{
		ParamContainer->AddSlot(i++,j)
			//.AutoHeight()
		[
			GetScalarParamSlot(item.Key, item.Value)
		];
	}

	//暂时不需要button,可以选择材质球上的clear删除
	/*ParamContainer->AddSlot(i++, j)
	[
		SNew(SButton)
		.Text(FText::FromString(TEXT("Delete")))
		.OnClicked(this,&SMaterialGroupItemWidget::DeleteCurrentMat)
	]
	;*/
}

FReply SMaterialGroupItemWidget::DeleteCurrentMat()
{

	DelegateManager::Get()->DeleteSceneMatInstance.Broadcast(m_MaterialInfo);
	return FReply::Handled();
}
