// Fill out your copyright notice in the Description page of Project Settings.


#include "SSceneManagerEntry.h"
#include "SPostProcessManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "DelegateManager.h"

#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "FSceneManagerModule"



void SSceneManagerTools::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

}

void SSceneManagerTools::Construct(const FArguments& InArgs)
{

	TSharedRef<SVerticalBox> Tabs = SNew(SVerticalBox).Visibility(this, &SSceneManagerTools::GetTabsVisibility);
	// Populate the tabs and body from the defined placeable items
	UE_LOG(LogTemp,Warning,TEXT("SSceneManagerTools Construct !"));

	SceneCategoryInfo Category;
	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Scene Material")));
	Category.UniqueHandle = FName(TEXT("SceneMaterial"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Scene Light")));
	Category.UniqueHandle = FName(TEXT("SceneLight"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Player Light")));
	Category.UniqueHandle = FName(TEXT("PlayerLight"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Post Process")));
	Category.UniqueHandle = FName(TEXT("PostProcess"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Wind")));
	Category.UniqueHandle = FName(TEXT("Wind"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Weather")));
	Category.UniqueHandle = FName(TEXT("Weather"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Tools")));
	Category.UniqueHandle = FName(TEXT("Tools"));
	Categories.Emplace(Category);


	for (int32 i = 0; i < Categories.Num(); i++)
	{

		Tabs->AddSlot()
		.AutoHeight()
		[
			CreatePlacementGroupTab(Categories[i])
		];
	}

	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar)
		.Thickness(FVector2D(5.0f, 5.0f))
		.AlwaysShowScrollbar(false)
		;

	ChildSlot
	[
		SNew(SVerticalBox)
		
		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				Tabs
			]

			+ SHorizontalBox::Slot()
			[
				SNew(SBorder)
				.Padding(FMargin(3))
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
				[

					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					[
						SAssignNew(SceneMaterialManager, SSceneMaterialManager)
					]
					
					+ SVerticalBox::Slot()
					[
						SAssignNew(PostProcessManager,SPostProcessManager)
					]

					+ SVerticalBox::Slot()
					[
						SAssignNew(SceneLightManager, SPlayerLightManager, TEXT("/Game/MPC_Scene.MPC_Scene"))
					]

					+ SVerticalBox::Slot()
					[
						SAssignNew(PlayerLightManager, SPlayerLightManager, TEXT("/Game/MPC_Player.MPC_Player"))
					]
				]
			]
		]
	];
	ActiveTabName = Categories[0].UniqueHandle;
	SelectManagerContainer("SceneMaterial");
}



TSharedRef< SWidget > SSceneManagerTools::CreatePlacementGroupTab(const SceneCategoryInfo& Info)
{
	return SNew(SCheckBox)
		.Style(FEditorStyle::Get(), "PlacementBrowser.Tab")
		.OnCheckStateChanged(this, &SSceneManagerTools::OnPlacementTabChanged, Info.UniqueHandle)
		.IsChecked(this, &SSceneManagerTools::GetPlacementTabCheckedState, Info.UniqueHandle)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpacer)
			.Size(FVector2D(1, 30))
		]

		+ SOverlay::Slot()
		.Padding(FMargin(6, 0, 15, 0))
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
			.Text(Info.DisplayName)
		]

		+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Left)
		[
			SNew(SImage)
			.Image(this, &SSceneManagerTools::PlacementGroupBorderImage, Info.UniqueHandle)
		]
		];


}

const FSlateBrush* SSceneManagerTools::PlacementGroupBorderImage(FName CategoryName) const
{
	if (ActiveTabName == CategoryName)
	{
		static FName PlacementBrowserActiveTabBarBrush("PlacementBrowser.ActiveTabBar");
		return FEditorStyle::GetBrush(PlacementBrowserActiveTabBarBrush);
	}
	else
	{
		return nullptr;
	}
}

void SSceneManagerTools::OnPlacementTabChanged(ECheckBoxState NewState, FName CategoryName)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ActiveTabName = CategoryName;
		SelectManagerContainer(CategoryName.ToString());
	}
}

void SSceneManagerTools::SelectManagerContainer(FString ContainerName)
{
	if (ContainerName == "SceneMaterial")
	{
		SceneMaterialManager->SetVisibility(EVisibility::Visible);
		PostProcessManager->SetVisibility(EVisibility::Collapsed);
		PlayerLightManager->SetVisibility(EVisibility::Collapsed);
		SceneLightManager->SetVisibility(EVisibility::Collapsed);
	}
	else if (ContainerName == "PostProcess")
	{
		PostProcessManager->SetVisibility(EVisibility::Visible);
		SceneMaterialManager->SetVisibility(EVisibility::Collapsed);
		PlayerLightManager->SetVisibility(EVisibility::Collapsed);
		SceneLightManager->SetVisibility(EVisibility::Collapsed);
	}
	else if (ContainerName == "SceneLight")
	{
		PostProcessManager->SetVisibility(EVisibility::Collapsed);
		SceneMaterialManager->SetVisibility(EVisibility::Collapsed);
		PlayerLightManager->SetVisibility(EVisibility::Collapsed);
		SceneLightManager->SetVisibility(EVisibility::Visible);
	}
	else if (ContainerName == "PlayerLight")
	{
		PostProcessManager->SetVisibility(EVisibility::Collapsed);
		SceneMaterialManager->SetVisibility(EVisibility::Collapsed);
		PlayerLightManager->SetVisibility(EVisibility::Visible);
		SceneLightManager->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		PostProcessManager->SetVisibility(EVisibility::Collapsed);
		SceneMaterialManager->SetVisibility(EVisibility::Collapsed);
		PlayerLightManager->SetVisibility(EVisibility::Collapsed);
		SceneLightManager->SetVisibility(EVisibility::Collapsed);
	}
}



ECheckBoxState SSceneManagerTools::GetPlacementTabCheckedState(FName CategoryName) const
{
	return ActiveTabName == CategoryName ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

EVisibility SSceneManagerTools::GetTabsVisibility() const
{
	return /*IsSearchActive() ? EVisibility::Collapsed :*/ EVisibility::Visible;
}




#undef LOCTEXT_NAMESPACE

