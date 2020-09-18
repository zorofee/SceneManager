// Fill out your copyright notice in the Description page of Project Settings.


#include "SSceneManagerEntry.h"
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
	if (bNeedsUpdate)
	{
		UpdateCategoryContent();
	}

	//Timer++;
	//if (Timer > 240)
	//{
	//	//自动保存
	//	DelegateManager::Get()->SaveGameData.Broadcast();
	//	Timer = 0;
	//}

}

void SSceneManagerTools::Construct(const FArguments& InArgs)
{

	MatGroupItems.Empty();

	TSharedRef<SVerticalBox> Tabs = SNew(SVerticalBox).Visibility(this, &SSceneManagerTools::GetTabsVisibility);
	// Populate the tabs and body from the defined placeable items
	UE_LOG(LogTemp,Warning,TEXT("SSceneManagerTools Construct !"));

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


	FString planName3 = FString::Printf(TEXT("BluePlan"));
	FString planName2 = FString::Printf(TEXT("GreenPlan"));
	FString planName1 = FString::Printf(TEXT("RedPlan"));
	FString planName4 = FString::Printf(TEXT("YellowPlan"));
	SourceComboList.Add(MakeShared<FString>(planName1));
	SourceComboList.Add(MakeShared<FString>(planName2));
	SourceComboList.Add(MakeShared<FString>(planName3));
	SourceComboList.Add(MakeShared<FString>(planName4));


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
					
					//选择层
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString("+ New Plan"))
							.OnClicked(this, &SSceneManagerTools::OnAddPlanNameButtonClicked)
							.ContentPadding(4.0f)
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(20, 0, 0, 0)
						[
							SAssignNew(PlanNameText, SEditableTextBox)
							.HintText(FText::FromString("Add new plan name"))
						]
						

						/*
						//测试用
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("TestSaveData")))
							.OnClicked(this, &SSceneManagerTools::TestSaveData)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString(TEXT("TestReadData")))
							.OnClicked(this, &SSceneManagerTools::TestReadData)
						]*/
					]
					
					//层名称
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						
						+SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(10,10)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Current Plan")))
						]
						
						+SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(10, 20)
						.VAlign(EVerticalAlignment::VAlign_Center)
						[
							SAssignNew(PlanComboBox,SComboBox<TSharedPtr<FString>>)
							.OptionsSource(&SourceComboList)
							.OnGenerateWidget(this, &SSceneManagerTools::GenerateSourceComboItem)
							.OnSelectionChanged(this, &SSceneManagerTools::HandleSourceComboChanged)
							[
								SAssignNew(ComboBoxSelectedText,STextBlock)
								.Text(FText::FromString(FString::Printf(TEXT("Default"))))
							]
						]
					]


					//材质管理面板
					+ SVerticalBox::Slot()
					[
						SAssignNew(LevelMaterialContent, SBox)
						.Visibility(EVisibility::Visible)
						[
							SNew(SHorizontalBox)
			
							+ SHorizontalBox::Slot()
							[
								SNew(SScrollBox)
								.ExternalScrollbar(ScrollBar)
								+SScrollBox::Slot()
								[

									SNew(SVerticalBox)

									//material group list
									+ SVerticalBox::Slot()
									[
										SAssignNew(ListView, SListView<TSharedPtr<FMaterialGroupInfo>>)
										.ListItemsSource(&MatGroupItems)
										.OnGenerateRow(this, &SSceneManagerTools::OnGenerateWidgetForItem)
									]

									
								]
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ScrollBar
							]
						]


					]



					//Add Group Button
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(10, 10, 0, 0)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.Text(FText::FromString("+ New Group"))
							.OnClicked(this, &SSceneManagerTools::OnAddGroupNameButtonClicked)
							.ContentPadding(4.0f)
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(10, 0, 0, 0)
						[
							SAssignNew(GroupNameText, SEditableTextBox)
							.HintText(FText::FromString("Add new group name"))
						]
					]








					/*
					//其他的面板
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
					*/







				]
			]
		]
	];

	ActiveTabName = Categories[0].UniqueHandle;

}

FReply SSceneManagerTools::OnAddPlanNameButtonClicked()
{
	FString planName = PlanNameText->GetText().ToString();
	if (planName != TEXT(""))
	{
		UE_LOG(LogTemp, Warning, TEXT("Add plan "));
		//SourceComboList.Emplace(*PlanNameText->GetText().ToString());
		SourceComboList.Emplace(MakeShared<FString>(planName));
		PlanComboBox->RefreshOptions();
	}
	return FReply::Handled();
}

TSharedRef<SWidget> SSceneManagerTools::GenerateSourceComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

void SSceneManagerTools::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		PlanComboBox->SetSelectedItem(Item);
		ComboBoxSelectedText->SetText(FText::FromString(*Item));
		DelegateManager::Get()->RefreshPlanList.Broadcast(*Item);
	}
}

TSharedRef<ITableRow> SSceneManagerTools::OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialGroupInfo>>, OwnerTable)
	[
		SNew(SMaterialGroupEntry, InItem.ToSharedRef())
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


FReply SSceneManagerTools::OnAddGroupNameButtonClicked()
{
	FString groupName = GroupNameText->GetText().ToString();
	if (groupName != TEXT(""))
	{
		TSharedPtr<FMaterialGroupInfo> GroupInfo = MakeShareable(new FMaterialGroupInfo());
		GroupInfo->GroupName = groupName;
		MatGroupItems.Emplace(GroupInfo);

		/*这一步要在刷新之前,因为要设置ParentPlan*/
		DelegateManager::Get()->AddSceneMatGroup.Broadcast(GroupInfo);
		ListView->RequestListRefresh();
	}
	return FReply::Handled();
}



void SSceneManagerTools::AddMaterialGroup(const FMaterialGroupInfo& groupInfo)
{
	TSharedPtr<FMaterialGroupInfo> GroupInfo = MakeShareable(new FMaterialGroupInfo());

	GroupInfo->GroupName = groupInfo.GroupName;
	GroupInfo->Parent = groupInfo.Parent;
	GroupInfo->MatList = groupInfo.MatList;
	MatGroupItems.Emplace(GroupInfo);
	ListView->RequestListRefresh();
}


FReply SSceneManagerTools::OnPlan1Clicked()
{
	DelegateManager::Get()->RefreshPlanList.Broadcast(TEXT("RedPlan"));

	return FReply::Handled();
}

FReply SSceneManagerTools::OnPlan2Clicked()
{
	DelegateManager::Get()->RefreshPlanList.Broadcast(TEXT("GreenPlan"));

	return FReply::Handled();
}

FReply SSceneManagerTools::OnPlan3Clicked()
{
	DelegateManager::Get()->RefreshPlanList.Broadcast(TEXT("BluePlan"));

	return FReply::Handled();
}

FReply SSceneManagerTools::TestSaveData()
{
	DelegateManager::Get()->SaveGameData.Broadcast();
	return FReply::Handled();
}

FReply SSceneManagerTools::TestReadData()
{
	DelegateManager::Get()->LoadGameData.Broadcast(TEXT("RedPlan"));
	return FReply::Handled();
}

void SSceneManagerTools::ClearMaterialGroup()
{
	MatGroupItems.Empty();
	ListView->RequestListRefresh();
}


#undef LOCTEXT_NAMESPACE

