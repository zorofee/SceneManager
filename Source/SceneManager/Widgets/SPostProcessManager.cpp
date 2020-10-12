// Fill out your copyright notice in the Description page of Project Settings.


#include "SPostProcessManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Scene.h"
#include "Widgets/Input/SComboBox.h"
#include "SaveDataManager.h"
#include "SWidgetTreasureBox.h"
#include "DelegateManager.h"

void SPostProcessManager::Construct(const FArguments& InArgs)
{
}

void SPostProcessManager::RefreshContentList()
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> PostProcessView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	PostProcessView->OnFinishedChangingProperties().AddRaw(this, &SPostProcessManager::OnFinishedChangingProperties);
	

	PostProcessView->SetObject(SaveDataManager::Get()->PostProcessSetting);

	ChildSlot
	[
		SNew(SVerticalBox)
		

		//选择层
		/*
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SPlanDropList)
			.Type(EPlanListType::PostProcess)
			//.OnAddPlan(this,&SPostProcessManager::OnAddPlan)
		]
		*/

		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0,20,0,0)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
				.Text(FText::FromString(TEXT("选择后处理体积:")))
			]

			+ SHorizontalBox::Slot()
			.Padding(10, 0, 0, 0)
			[
				SAssignNew(PostProcessComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&SourceComboList)
				.OnGenerateWidget(this, &SPostProcessManager::GenerateSourceComboItem)
				.OnSelectionChanged(this, &SPostProcessManager::HandleSourceComboChanged)
				.OnComboBoxOpening(this, &SPostProcessManager::OnComboBoxOpening)
				[
					SAssignNew(ComboBoxSelectedText, STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT("Default"))))
				]
			]
		]

		+SVerticalBox::Slot()
		[
			PostProcessView
		]
	];
}


void SPostProcessManager::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	if (SelectedPostProcess != nullptr)
	{
		SetPostProcessParams(*SelectedPostProcess);
	}
}

void SPostProcessManager::GetScenePostProcessVolume()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Actor num is %d"), actorList.Num());
	
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(actorList[i]);
		FString ppName = PostProcessVolume->GetName();
		FString selectedName = *PostProcessComboBox->GetSelectedItem().Get();
		if (ppName.Equals(selectedName))
		{
			SelectedPostProcess = PostProcessVolume;
			GetPostProcessParams(*PostProcessVolume);
		}
	}
}



void SPostProcessManager::SetPostProcessParams(APostProcessVolume& Volume)
{
	if (!Volume.IsValidLowLevel())
	{
		UE_LOG(LogTemp,Warning,TEXT("Post process is not valid"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());
	Volume.Settings = SaveDataManager::Get()->PostProcessSetting->Setting;
}


void SPostProcessManager::GetPostProcessParams(APostProcessVolume& Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());
	SaveDataManager::Get()->PostProcessSetting->Setting = Volume.Settings;
}

void SPostProcessManager::GetPostProcessParams(FPostProcessSettings setting)
{
	SaveDataManager::Get()->PostProcessSetting->Setting = setting;
}


TSharedRef<SWidget> SPostProcessManager::GenerateSourceComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}


void SPostProcessManager::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		PostProcessComboBox->SetSelectedItem(Item);
		ComboBoxSelectedText->SetText(FText::FromString(*Item.Get()));

		GetScenePostProcessVolume();
	}
}

void SPostProcessManager::OnComboBoxOpening()
{
	SourceComboList.Empty();
	RefreshPostProcessComboList();
	PostProcessComboBox->RefreshOptions();
}

void SPostProcessManager::RefreshPostProcessComboList()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Postprocess num is %d"), actorList.Num());
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		SourceComboList.Emplace(MakeShareable(new FString(actorList[i]->GetName())));
	}
}


FReply SPostProcessManager::OnAddPlan()
{
	UE_LOG(LogTemp,Warning,TEXT("Post process manager add plan"));
	return FReply::Handled();
}
