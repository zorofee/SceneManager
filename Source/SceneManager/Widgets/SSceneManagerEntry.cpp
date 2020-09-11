// Fill out your copyright notice in the Description page of Project Settings.


#include "SSceneManagerEntry.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "SMaterialGroupWidget.h"

#define LOCTEXT_NAMESPACE "FSceneManagerModule"



void SMaterialAssetEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroup>& InItem)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0,10,0,10)
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
				.Text(FText::FromString(InItem->GroupName))
				
			]

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(2, 0, 4, 0)
			[
				SNew(SMaterialGroupWidget)
			]
		]
	];
}


void SSceneManagerTools::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (bNeedsUpdate)
	{
		UpdateCategoryContent();
	}
}

void SSceneManagerTools::Construct(const FArguments& InArgs)
{
	TSharedRef<SVerticalBox> Tabs = SNew(SVerticalBox).Visibility(this, &SSceneManagerTools::GetTabsVisibility);
	// Populate the tabs and body from the defined placeable items


	SceneCategoryInfo Category;
	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Level Material")));
	Category.UniqueHandle = FName(TEXT("LevelMaterial"));
	Categories.Emplace(Category);

	Category.DisplayName = FText::FromString(FString::Printf(TEXT("Level Light")));
	Category.UniqueHandle = FName(TEXT("LevelLight"));
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
		.Thickness(FVector2D(9.0f, 9.0f));



	TSharedPtr<FMaterialGroup> Group0 = MakeShareable(new FMaterialGroup());
	Group0->GroupName = FString::Printf(TEXT("Stone"));
	FilteredItems.Emplace(Group0);

	TSharedPtr<FMaterialGroup> Group1 = MakeShareable(new FMaterialGroup());
	Group1->GroupName = FString::Printf(TEXT("Grass"));
	FilteredItems.Emplace(Group1);

	TSharedPtr<FMaterialGroup> Group2 = MakeShareable(new FMaterialGroup());
	Group2->GroupName = FString::Printf(TEXT("Tree"));
	FilteredItems.Emplace(Group2);

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
					
					//Ñ¡Ôñ²ã
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("plan1")))
						
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("plan2")))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("+")))
						]

					]
					
					//²ãÃû³Æ
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Name")))
						]
						
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SEditableText)
						]
					]


					+ SVerticalBox::Slot()
					[
						SAssignNew(LevelMaterialContent, SBox)
						.Visibility(EVisibility::Visible)
						[
							SNew(SHorizontalBox)

							//²ÄÖÊÇò
							+ SHorizontalBox::Slot()
							[
								SNew(SVerticalBox)

								//material group list
								+ SVerticalBox::Slot()
								[
									SAssignNew(ListView, SListView<TSharedPtr<FMaterialGroup>>)
									.ListItemsSource(&FilteredItems)
									.OnGenerateRow(this, &SSceneManagerTools::OnGenerateWidgetForItem)
									.ExternalScrollbar(ScrollBar)
								]
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ScrollBar
							]
						]

					]


					+ SVerticalBox::Slot()
					[
						SAssignNew(LevelLightContent, SBox)
						.Visibility(EVisibility::Collapsed)
						[
							SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Anything else")))
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ScrollBar
							]
						]
					]

				]
			]
		]
	];

	ActiveTabName = Categories[0].UniqueHandle;
}



TSharedRef<ITableRow> SSceneManagerTools::OnGenerateWidgetForItem(TSharedPtr<FMaterialGroup> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialGroup>>, OwnerTable)
		[
			SNew(SMaterialAssetEntry, InItem.ToSharedRef())
			//.HighlightText(this, &SSceneManagerTools::GetHighlightText)
		];
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
		//IPlacementModeModule::Get().RegenerateItemsForCategory(ActiveTabName);

		bNeedsUpdate = true;
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


void SSceneManagerTools::OnSearchChanged(const FText& InFilterText)
{
	UE_LOG(LogTemp, Warning, TEXT("On Search Changed"));
}

void SSceneManagerTools::OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType)
{
	UE_LOG(LogTemp, Warning, TEXT("On Search Committed"));

}

void SSceneManagerTools::UpdateCategoryContent()
{
	bNeedsUpdate = false;


	//CustomContent->SetVisibility(EVisibility::Visible);
	if (ActiveTabName == Categories[0].UniqueHandle)
	{
		LevelMaterialContent->SetVisibility(EVisibility::Visible);
		LevelLightContent->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		LevelMaterialContent->SetVisibility(EVisibility::Collapsed);
		LevelLightContent->SetVisibility(EVisibility::Visible);
	}
	
}
#undef LOCTEXT_NAMESPACE

