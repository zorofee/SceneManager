// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerLightManager.h"
#include "SMaterialGroupWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LightComponent.h"
#include "Engine/World.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"

void SPlayerLightManager::Construct(const FArguments& InArgs)
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	UMaterialParameterCollection* loadasset = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL, TEXT("/Game/MPC_Player.MPC_Player")));


	if (loadasset != nullptr)
	{
		MPC = loadasset;
		//UMaterialParameterCollectionInstance* ParameterCollectionInstance = GEditor->GetEditorWorldContext().World()->GetParameterCollectionInstance(MPC);
		//ParameterCollectionInstance->SetScalarParameterValue("PLAYER", 3.14f);
		
	}
	else
	{
		MPC = NewObject<UMaterialParameterCollection>();
	}

	TSharedRef<IDetailsView> MPCView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	MPCView->SetObject(MPC);



	TSharedRef<IDetailsView> PlayerLightView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	PlayerLightView->OnFinishedChangingProperties().AddRaw(this, &SPlayerLightManager::OnFinishedChangingMainLight);
	LightSetting = NewObject<UPlayerLightSettings>();
	PlayerLightView->SetObject(LightSetting);


	//GetAllSceneLight();

	ChildSlot
	[
		SNew(SVerticalBox)
		
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(PlanComboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&SourceComboList)
			.OnGenerateWidget(this, &SPlayerLightManager::GenerateSourceComboItem)
			.OnSelectionChanged(this, &SPlayerLightManager::HandleSourceComboChanged)
			.OnComboBoxOpening(this,&SPlayerLightManager::OnComboBoxOpening)
			[
				SAssignNew(ComboBoxSelectedText, STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("Default"))))
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			PlayerLightView
		]
		
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//MPCView
			SAssignNew(ParamsContainer,SVerticalBox)
		]
	];


	ColorImageArray.Empty();
	for (size_t i = 0; i < MPC->ScalarParameters.Num(); i++)
	{
		AddScalarParam(MPC->ScalarParameters[i].ParameterName.ToString(), MPC->ScalarParameters[i].DefaultValue);
	}

	for (size_t i = 0; i < MPC->VectorParameters.Num(); i++)
	{
		AddVectorParam(MPC->VectorParameters[i].ParameterName.ToString(), MPC->VectorParameters[i].DefaultValue,i);
	}
}

void SPlayerLightManager::OnFinishedChangingMainLight(const FPropertyChangedEvent& InEvent)
{
	SelectedLight->SetLightColor(LightSetting->color);
	SelectedLight->GetLightComponent()->SetIntensity(LightSetting->intensity);
	SelectedLight->GetLightComponent()->SetWorldRotation(LightSetting->rotation);
}


void SPlayerLightManager::GetAllSceneLight()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, ALight::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Actor num is %d"), actorList.Num());
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		SourceComboList.Emplace(MakeShareable(new FString(actorList[i]->GetName())));
	}
}

void SPlayerLightManager::FindSceneLight(const FString lightName)
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, ALight::StaticClass(), actorList);
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		if (actorList[i]->GetName() == lightName)
		{
			SelectedLight = Cast<ALight>(actorList[i]);
			LightSetting->color = SelectedLight->GetLightColor();
			LightSetting->intensity = SelectedLight->GetLightComponent()->Intensity;
			LightSetting->rotation = SelectedLight->GetLightComponent()->GetComponentRotation();
		}
	}
}


TSharedRef<SWidget> SPlayerLightManager::GenerateSourceComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}


void SPlayerLightManager::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		PlanComboBox->SetSelectedItem(Item);
		ComboBoxSelectedText->SetText(FText::FromString(*Item.Get()));
		FindSceneLight(*Item);
	}
}

void SPlayerLightManager::OnComboBoxOpening()
{
	SourceComboList.Empty();
	GetAllSceneLight();
	PlanComboBox->RefreshOptions();
}

void SPlayerLightManager::AddScalarParam(const FString name, float value)
{
	ParamsContainer->AddSlot()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10, 10, 10, 10)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(name))
		]

		+ SHorizontalBox::Slot()
		.Padding(10, 10, 10, 10)
		[
			SNew(SSpinBox<float>)
			.OnValueChanged(this, &SPlayerLightManager::OnScalarValueChanged, name)
			//.OnValueCommitted(this, &SPlayerLightManager::OnScalarValueCommitted, name)
			.MinValue(0)
			.MaxValue(1)
			.Value(value)
		]
	];
}


void SPlayerLightManager::AddVectorParam(const FString name, FLinearColor value,int32 index)
{
	TSharedPtr<SImage> ColorImage;
	ColorImageArray.Emplace(ColorImage);

	ParamsContainer->AddSlot()
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10, 10, 10, 10)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(name))
		]

		+ SHorizontalBox::Slot()
		.Padding(10, 10, 10, 10)
		[
			SAssignNew(ColorImageArray[index], SImage)
			.ColorAndOpacity(FSlateColor(value))
			.OnMouseButtonDown(this, &SPlayerLightManager::OnClickColorBlock, name, value, index)
		]
	];
}


FReply SPlayerLightManager::OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name, FLinearColor defaultColor, int32 ColorImageIndex)
{
	FColorPickerArgs PickerArgs;
	{
		PickerArgs.bUseAlpha = true;
		PickerArgs.bOnlyRefreshOnOk = false;
		PickerArgs.bOnlyRefreshOnMouseUp = true;

		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &SPlayerLightManager::OnSetColorFromColorPicker, name, ColorImageIndex);
		PickerArgs.OnColorPickerWindowClosed = FOnWindowClosed::CreateSP(this, &SPlayerLightManager::OnColorPickerWindowClosed);
		PickerArgs.InitialColorOverride = defaultColor;
		PickerArgs.ParentWidget = ColorImageArray[ColorImageIndex];
	}

	OpenColorPicker(PickerArgs);
	return FReply::Handled();
}

void SPlayerLightManager::OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window)
{

}

void SPlayerLightManager::OnSetColorFromColorPicker(FLinearColor NewColor, FString name, int32 ColorImageIndex)
{
	ColorImageArray[ColorImageIndex]->SetColorAndOpacity(NewColor);
	OnScalarVectorChanged(NewColor,name);
}


void SPlayerLightManager::OnScalarValueChanged(float value, const FString name)
{
	for (size_t i = 0; i < MPC->ScalarParameters.Num(); i++)
	{
		if (MPC->ScalarParameters[i].ParameterName.ToString() == name)
		{
			MPC->ScalarParameters[i].DefaultValue = value;
		}
	}
}

void SPlayerLightManager::OnScalarVectorChanged(FLinearColor value,const FString name)
{
	for (size_t i = 0; i < MPC->VectorParameters.Num(); i++)
	{
		if (MPC->VectorParameters[i].ParameterName.ToString() == name)
		{
			MPC->VectorParameters[i].DefaultValue = value;
		}
	}
}
