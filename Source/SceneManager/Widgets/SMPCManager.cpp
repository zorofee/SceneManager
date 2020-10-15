// Fill out your copyright notice in the Description page of Project Settings.


#include "SMPCManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LightComponent.h"
#include "Engine/World.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"

void SMPCManager::Construct(const FArguments& InArgs, const FString MPCPath)
{
	TempMPCPath = MPCPath;
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	UMaterialParameterCollection* ParameterCollection = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL,*MPCPath));

	if (ParameterCollection != nullptr)
	{

		MPC = ParameterCollection;
		
	}
	else
	{
		MPC = NewObject<UMaterialParameterCollection>();
	}


	TSharedRef<IDetailsView> MPCView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	MPCView->SetObject(MPC);
	


	ChildSlot
	[
		SNew(SVerticalBox)
		
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10,20,0,0)
		[
			SNew(SBorder)
			[
				SNew(SVerticalBox)
				
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
					.Text(FText::FromString(TEXT("全局参数")))
				]
				
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					//MPCView
					SAssignNew(ParamsContainer, SVerticalBox)
				]
			]
		]
	];


	RefreshMPCWidgets();
}



void SMPCManager::RefreshMPCWidgets()
{
	ColorImageArray.Empty();
	for (size_t i = 0; i < MPC->ScalarParameters.Num(); i++)
	{
		AddScalarParam(MPC->ScalarParameters[i].ParameterName.ToString(), MPC->ScalarParameters[i].DefaultValue);
	}

	for (size_t i = 0; i < MPC->VectorParameters.Num(); i++)
	{
		AddVectorParam(MPC->VectorParameters[i].ParameterName.ToString(), MPC->VectorParameters[i].DefaultValue, i);
	}
}



void SMPCManager::SaveMPCParams()
{
	FString AssetPath  = TempMPCPath;
	TArray<FString> splitPath;
	AssetPath.ParseIntoArray(splitPath, TEXT("."));
	FString PackagePath = splitPath[0];
	FString FileName = splitPath[1];
	FString FilePath = AssetPath.Replace(TEXT("/Game/"), *FPaths::ProjectContentDir()).Replace(*FString::Printf(TEXT(".%s"), *FileName), TEXT(".uasset"));
	UE_LOG(LogTemp, Warning, TEXT("SaveMaterialInstance %s ,  %s"), *PackagePath, *FilePath);
	GEngine->Exec(NULL, *FString::Printf(TEXT("OBJ SAVEPACKAGE PACKAGE=\"%s\" FILE=\"%s\" SILENT=true"), *PackagePath, *FilePath));
}




void SMPCManager::AddScalarParam(const FString name, float value)
{
	ParamsContainer->AddSlot()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5, 5, 5, 5)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(name))
		]

		+ SHorizontalBox::Slot()
		.Padding(5, 5, 5, 5)
		[
			SNew(SSpinBox<float>)
			.OnValueChanged(this, &SMPCManager::OnScalarValueChanged, name)
			.OnValueCommitted(this, &SMPCManager::OnScalarValueCommited)
			.MinValue(0)
			.MaxValue(1)
			.Value(value)
		]
	];
}


void SMPCManager::AddVectorParam(const FString name, FLinearColor value,int32 index)
{
	TSharedPtr<SImage> ColorImage;
	ColorImageArray.Emplace(ColorImage);

	ParamsContainer->AddSlot()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5, 5, 5, 5)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(name))
		]

		+ SHorizontalBox::Slot()
		.Padding(5, 5, 5, 5)
		[
			SAssignNew(ColorImageArray[index], SImage)
			.ColorAndOpacity(FSlateColor(FLinearColor(value.R, value.G, value.B, 1.0f)))
			.OnMouseButtonDown(this, &SMPCManager::OnClickColorBlock, name, value, index)
		]
	];
}


FReply SMPCManager::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name, FLinearColor defaultColor, int32 ColorImageIndex)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.bOnlyRefreshOnMouseUp = true;

		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SMPCManager::OnSetColorFromColorPicker, name, ColorImageIndex);
		PickerArgs.OnColorPickerWindowClosed = FOnWindowClosed::CreateSP(this, &SMPCManager::OnColorPickerWindowClosed);
		PickerArgs.InitialColorOverride = defaultColor;
		PickerArgs.ParentWidget = ColorImageArray[ColorImageIndex];
	}
	OpenColorPicker(PickerArgs);
	return FReply::Handled();
}

void SMPCManager::OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window)
{
	SaveMPCParams();
}

void SMPCManager::OnSetColorFromColorPicker(FLinearColor NewColor, FString name, int32 ColorImageIndex)
{
	ColorImageArray[ColorImageIndex]->SetColorAndOpacity(NewColor);
	OnVectorValueChanged(NewColor,name);
}


void SMPCManager::OnScalarValueChanged(float value, const FString name)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	MPCIns = World->GetParameterCollectionInstance(MPC);
	for (size_t i = 0; i < MPC->ScalarParameters.Num(); i++)
	{
		if (MPC->ScalarParameters[i].ParameterName.ToString() == name)
		{
			MPC->ScalarParameters[i].DefaultValue = value;
			MPCIns->SetScalarParameterValue(MPC->ScalarParameters[i].ParameterName,value);
		}
	}
}

void SMPCManager::OnScalarValueCommited(float NewValue, ETextCommit::Type CommitType)
{
	SaveMPCParams();
}


void SMPCManager::OnVectorValueChanged(FLinearColor value,const FString name)
{

	UWorld* World = GEditor->GetEditorWorldContext().World();
	MPCIns = World->GetParameterCollectionInstance(MPC);
	for (size_t i = 0; i < MPC->VectorParameters.Num(); i++)
	{
		if (MPC->VectorParameters[i].ParameterName.ToString() == name)
		{
			MPC->VectorParameters[i].DefaultValue = value;
			MPCIns->SetVectorParameterValue(MPC->VectorParameters[i].ParameterName, value);
		}
	}
}
