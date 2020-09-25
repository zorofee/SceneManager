// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupItemWidget.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Materials/MaterialInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SceneManagerSaveGame.h"
#include "Materials/MaterialInstanceConstant.h"
#include "DelegateManager.h"


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
	GroupNameSet.Emplace(TEXT("LevelMaterial"));
	GroupNameSet.Emplace(TEXT("Ref"));
	GroupNameSet.Emplace(TEXT("辅光"));
	GroupNameSet.Emplace(TEXT("HSV"));

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

	
		//测试用
		/*+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.OnClicked(this, &SMaterialGroupItemWidget::OnTest)
		]
		*/
		];

	
	//如果已有材质路径则直接根据已有的数据刷新参数
	if (m_MaterialInfo->MatPath != TEXT(""))
	{
		LoadMaterialInstanceByInfo();
	}

}

FReply SMaterialGroupItemWidget::OnTest()
{
	UE_LOG(LogTemp,Warning,TEXT("Material instance is %s , %s"),*lms->material->GetPathName(),*m_MaterialInfo->MatPath);
	UObject* loadMat = StaticLoadObject(UMaterialInstanceConstant::StaticClass(), NULL, TEXT("/Game/Grass_Ins.Grass_Ins"));
	if (loadMat != nullptr)
	{
		//lms->material = Cast<UMaterialInstanceConstant>(loadMat);
	}
	return FReply::Handled();
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
	DelegateManager::Get()->SelectMaterialInstance.Broadcast(lms->material->GetPathName(),m_MaterialInfo,this);
}

void SMaterialGroupItemWidget::ChangeSelectedMatInstance()
{
	FString originPath = m_MaterialInfo->MatPath;

	AnalysisMaterialParamsAndPath();

	//更新数据层
	DelegateManager::Get()->AddSceneMatInstance.Broadcast(m_MaterialInfo);
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
		.HAlign(EHorizontalAlignment::HAlign_Left)
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
		.Padding(10,0,10, 0)
		[
		SNew(SSpinBox<float>)
		.OnValueChanged(this, &SMaterialGroupItemWidget::OnScalarValueChanged, name)
		.OnValueCommitted(this, &SMaterialGroupItemWidget::OnScalarValueCommitted, name)
		.MinValue(0)
		.MaxValue(1)
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
	TSharedPtr<SImage> ColorImage;
	ColorImageArray.Emplace(ColorImage);
	int32 index = ColorImageArray.Num() - 1;
	return
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(EHorizontalAlignment::HAlign_Left)
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
	.Padding(10, 0, 10, 0)

	[
		SAssignNew(ColorImageArray[index], SImage)
		.ColorAndOpacity(FSlateColor(value))
		.OnMouseButtonDown(this, &SMaterialGroupItemWidget::OnClickColorBlock, name,value, index)
	];
}

void SMaterialGroupItemWidget::OnScalarValueChanged(float value, FString name)
{
	m_MaterialInfo->ScalarParams[name] = value;
	DelegateManager::Get()->OnMatParamChanged.Broadcast(m_MaterialInfo);

	SetMaterialInstanceScalarParam(name, value);
}

void SMaterialGroupItemWidget::OnScalarValueCommitted(float NewEqualValue, ETextCommit::Type CommitType, FString name)
{
	SaveMaterialInstance();
}


FReply SMaterialGroupItemWidget::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name, FLinearColor defaultColor, int32 ColorImageIndex)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.bOnlyRefreshOnMouseUp = true;

		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SMaterialGroupItemWidget::OnSetColorFromColorPicker, name, ColorImageIndex);
		PickerArgs.OnColorPickerWindowClosed = FOnWindowClosed::CreateSP(this, &SMaterialGroupItemWidget::OnColorPickerWindowClosed);
		PickerArgs.InitialColorOverride = defaultColor;
		PickerArgs.ParentWidget = ColorImageArray[ColorImageIndex];
	}

	OpenColorPicker(PickerArgs);
	return FReply::Handled();
}

void SMaterialGroupItemWidget::OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window)
{
	SaveMaterialInstance();
}

void SMaterialGroupItemWidget::OnSetColorFromColorPicker(FLinearColor NewColor,FString name, int32 ColorImageIndex)
{
	ColorImageArray[ColorImageIndex]->SetColorAndOpacity(NewColor);
	m_MaterialInfo->VectorParams[name] = NewColor;
	DelegateManager::Get()->OnMatParamChanged.Broadcast(m_MaterialInfo);

	SetMaterialInstanceVectorParam(name,NewColor);
}


void SMaterialGroupItemWidget::LoadMaterialInstanceByInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Load current material %s"), *m_MaterialInfo->MatPath);

	UObject* loadMat = StaticLoadObject(UMaterialInstance::StaticClass(), NULL, *m_MaterialInfo->MatPath);
	if (loadMat != nullptr)
	{
		lms->material = Cast<UMaterialInstance>(loadMat);
		
		/*先根据保存的数据创建出参数slot,这一步应该要跟当前材质的参数做对比*/
		AnalysisMaterialParamsAndPath();
		
		ResetMaterialInstanceConstant();
	}
}

/*在根据保存的数据刷一遍材质球*/
void SMaterialGroupItemWidget::ResetMaterialInstanceConstant()
{
	for (TPair<FString, FLinearColor> item : m_MaterialInfo->VectorParams)
	{
		SetMaterialInstanceVectorParam(item.Key, item.Value);
	}

	for (TPair<FString, float> item : m_MaterialInfo->ScalarParams)
	{
		SetMaterialInstanceScalarParam(item.Key, item.Value);
	}
}

/*在读取材质球时,将本地保存的数据与材质球上的数据做对比*/
void SMaterialGroupItemWidget::AnalysisMaterialParamsAndPath()
{
	ParamContainer.Get()->ClearChildren();
	ColorImageArray.Empty();

	TMap<FString, float> TempScalarParams;
	TMap<FString, FLinearColor> TempVectorParams;

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
			if (GroupNameSet.Contains(GroupName))
			{
				ParamName = ParameterInfo[i].Name.ToString();
				Material->GetScalarParameterValue(ParameterInfo[i], ScalarValue);
				TempScalarParams.Emplace(ParamName, ScalarValue);
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
			if (GroupNameSet.Contains(GroupName))
			{
				ParamName = ParameterInfo[i].Name.ToString();
				Material->GetVectorParameterValue(ParameterInfo[i], VectorValue);
				TempVectorParams.Emplace(ParamName, VectorValue);
			}
		}


		/*因为材质球可能有修改，因此在设置本地数据之前先要进行对比,将材质球和本地数据中都有的数据更新*/
		for (TPair<FString, float> item : TempScalarParams)
		{
			if (m_MaterialInfo->ScalarParams.Contains(item.Key))
			{
				TempScalarParams[item.Key] = m_MaterialInfo->ScalarParams[item.Key];
			}
		}

		for (TPair<FString, FLinearColor> item : TempVectorParams)
		{
			if (m_MaterialInfo->VectorParams.Contains(item.Key))
			{
				TempVectorParams[item.Key] = m_MaterialInfo->VectorParams[item.Key];
			}
		}

		m_MaterialInfo->ScalarParams = TempScalarParams;
		m_MaterialInfo->VectorParams = TempVectorParams;

		AddParamsToSlot();

		m_MaterialInfo->MatPath = Material->GetPathName();
	}
}



void SMaterialGroupItemWidget::AddParamsToSlot()
{
	int i = 0 , j = 0 , col = 2;
	for (TPair<FString, FLinearColor> item : m_MaterialInfo->VectorParams)
	{
		ParamContainer->AddSlot(i,j)
			//.AutoHeight()
		[
			GetVectorParamSlot(item.Key,item.Value)
		];
		i += 1;
		if (i == col)
		{
			i = 0;
			j++;
		}
	}

	for (TPair<FString, float> item : m_MaterialInfo->ScalarParams)
	{
		ParamContainer->AddSlot(i,j)
			//.AutoHeight()
		[
			GetScalarParamSlot(item.Key, item.Value)
		];
		i += 1;
		if (i == col)
		{
			i = 0;
			j++;
		}
	}
}


void SMaterialGroupItemWidget::SetMaterialInstanceScalarParam(FString name, float scalar)
{
	//修改资源浏览器中的材质实例
	UMaterialInstanceConstant* matInsConstant = Cast<UMaterialInstanceConstant>(lms->material);
	TArray<FMaterialParameterInfo> ParameterInfo;
	TArray<FGuid> ParameterGuids;
	FString GroupName;
	FString ParamName;

	//Get scalar parameters name and value
	matInsConstant->GetAllScalarParameterInfo(ParameterInfo, ParameterGuids);


	for (size_t i = 0; i < ParameterInfo.Num(); i++)
	{
		FName groupName;
		matInsConstant->GetGroupName(ParameterInfo[i], groupName);

		GroupName = groupName.ToString();
		if (GroupNameSet.Contains(GroupName))
		{
			ParamName = ParameterInfo[i].Name.ToString();
			if (ParamName == name)
			{
				matInsConstant->SetScalarParameterValueEditorOnly(ParameterInfo[i], scalar);
			}
		}
	}
}

void SMaterialGroupItemWidget::SetMaterialInstanceVectorParam(FString name, FLinearColor color)
{
	UE_LOG(LogTemp, Warning, TEXT("SetMaterialInstanceVectorParam "));
	//修改资源浏览器中的材质实例
	UMaterialInstanceConstant* matInsConstant = Cast<UMaterialInstanceConstant>(lms->material);
	TArray<FMaterialParameterInfo> ParameterInfo;
	TArray<FGuid> ParameterGuids;
	FString GroupName;
	FString ParamName;

	//Get scalar parameters name and value
	matInsConstant->GetAllVectorParameterInfo(ParameterInfo, ParameterGuids);


	for (size_t i = 0; i < ParameterInfo.Num(); i++)
	{
		FName groupName;
		matInsConstant->GetGroupName(ParameterInfo[i], groupName);

		GroupName = groupName.ToString();
		if (GroupNameSet.Contains(GroupName))
		{
			ParamName = ParameterInfo[i].Name.ToString();
			if (ParamName == name)
			{
				matInsConstant->SetVectorParameterValueEditorOnly(ParameterInfo[i], color);
			}
		}
	}
}

void SMaterialGroupItemWidget::SaveMaterialInstance()
{
	UMaterialInstanceConstant* matInsConstant = Cast<UMaterialInstanceConstant>(lms->material);
	FString matName = matInsConstant->GetName();
	FString matPath = matInsConstant->GetPathName();
	TArray<FString> splitPath;
	matPath.ParseIntoArray(splitPath, TEXT("."));
	FString PackagePath = splitPath[0];
	FString FileName = splitPath[1];
	FString FilePath = matPath.Replace(TEXT("/Game/"), *FPaths::ProjectContentDir()).Replace(*FString::Printf(TEXT(".%s"), *FileName), TEXT(".uasset"));
	UE_LOG(LogTemp, Warning, TEXT("SaveMaterialInstance %s ,  %s"), *PackagePath, *FilePath);
	GEngine->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *PackagePath, *FilePath));
}


void SMaterialGroupItemWidget::SelectedRepeatedMatInstance()
{
	ParamContainer.Get()->ClearChildren();
	ColorImageArray.Empty();
	lms->material = nullptr;
}
