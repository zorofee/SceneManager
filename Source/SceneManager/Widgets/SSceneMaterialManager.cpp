// Fill out your copyright notice in the Description page of Project Settings.


#include "SSceneMaterialManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "DelegateManager.h"

#include "Widgets/Layout/SScrollBox.h"



void SSceneMaterialManager::Construct(const FArguments& InArgs)
{
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar)
		.Thickness(FVector2D(5.0f, 5.0f))
		.AlwaysShowScrollbar(false)
		;

	ChildSlot
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
				.OnClicked(this, &SSceneMaterialManager::OnAddPlanNameButtonClicked)
				.ContentPadding(4.0f)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(20, 0, 0, 0)
			[
				SAssignNew(PlanNameText, SEditableTextBox)
				.HintText(FText::FromString("Add new plan name"))
			]
		]

		//层名称
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(10, 10)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Current Plan")))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(10, 20)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(PlanComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&SourceComboList)
				.OnGenerateWidget(this, &SSceneMaterialManager::GenerateSourceComboItem)
				.OnSelectionChanged(this, &SSceneMaterialManager::HandleSourceComboChanged)
				[
				SAssignNew(ComboBoxSelectedText, STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("Default"))))
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SButton)
				.OnClicked(this, &SSceneMaterialManager::OnDeleteCurrentPlan)
				.Text(FText::FromString(TEXT("Delete Current Plan")))

			]
		]


		//材质管理面板
		+SVerticalBox::Slot()
		[
			SAssignNew(SceneMaterialContent, SBox)
			.Visibility(EVisibility::Visible)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				[
					SNew(SScrollBox)
					.ExternalScrollbar(ScrollBar)
					+ SScrollBox::Slot()
					[
						SNew(SVerticalBox)

						//material group list
						+ SVerticalBox::Slot()
						[
							SAssignNew(MatGroupListView, SListView<TSharedPtr<FMaterialGroupInfo>>)
							.ListItemsSource(&MatGroupItems)
							.OnGenerateRow(this, &SSceneMaterialManager::OnGenerateWidgetForItem)
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
				.OnClicked(this, &SSceneMaterialManager::AddNewMaterialGroup)
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

	];

}




TSharedRef<SWidget> SSceneMaterialManager::GenerateSourceComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

FReply SSceneMaterialManager::OnAddPlanNameButtonClicked()
{
	FString planName = PlanNameText->GetText().ToString();
	if (planName != TEXT(""))
	{
		//不为空时添加成功
		DelegateManager::Get()->AddSceneMatPlan.Broadcast(planName);
	}
	return FReply::Handled();
}


FReply SSceneMaterialManager::OnDeleteCurrentPlan()
{
	TSharedPtr<FString> planName = PlanComboBox->GetSelectedItem();
	if (planName)
	{
		DelegateManager::Get()->DeleteSceneMatPlan.Broadcast(*planName.Get());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("OnDeleteCurrentPlan Failed!"));
	}
	return FReply::Handled();
}


void SSceneMaterialManager::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		PlanComboBox->SetSelectedItem(Item);
		SetSelectedPlanName(*Item);
		DelegateManager::Get()->RefreshPlanList.Broadcast(*Item);
	}
}




void SSceneMaterialManager::SetSelectedPlanName(const FString planName)
{
	for (size_t i = 0; i < SourceComboList.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSelectedPlanName %s"), *planName);
		if (*SourceComboList[i] == planName)
		{
			PlanComboBox->SetSelectedItem(SourceComboList[i]);
		}
	}
	ComboBoxSelectedText->SetText(FText::FromString(planName));
}

void SSceneMaterialManager::ResetPlanComboBox(const TArray<FString> planNameList)
{
	SourceComboList.Empty();
	for (size_t i = 0; i < planNameList.Num(); i++)
	{
		SourceComboList.Emplace(MakeShareable(new FString(planNameList[i])));
	}
	PlanComboBox->RefreshOptions();
}


TSharedRef<ITableRow> SSceneMaterialManager::OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialGroupInfo>>, OwnerTable)
		[
			SNew(SMaterialGroupEntry, InItem.ToSharedRef())
		];
}


FReply SSceneMaterialManager::AddNewMaterialGroup()
{
	FString groupName = GroupNameText->GetText().ToString();
	if (groupName != TEXT(""))
	{
		DelegateManager::Get()->AddSceneMatGroup.Broadcast(groupName);
	}
	return FReply::Handled();
}


void SSceneMaterialManager::AddMaterialGroup(const FMaterialGroupInfo& groupInfo)
{
	TSharedPtr<FMaterialGroupInfo> GroupInfo = MakeShareable(new FMaterialGroupInfo());

	GroupInfo->GroupName = groupInfo.GroupName;
	GroupInfo->Parent = groupInfo.Parent;
	GroupInfo->MatList = groupInfo.MatList;
	MatGroupItems.Emplace(GroupInfo);
	MatGroupListView->RequestListRefresh();

}

void SSceneMaterialManager::DeleteMaterialGroup(TSharedPtr<FMaterialGroupInfo> matInfo)
{
	for (size_t i = 0; i < MatGroupItems.Num(); i++)
	{
		if (MatGroupItems[i]->GroupName == matInfo->GroupName)
		{
			MatGroupItems.RemoveAt(i);
		}
	}

	MatGroupListView->RequestListRefresh();
}


void SSceneMaterialManager::ClearMaterialGroup()
{
	MatGroupItems.Empty();
	MatGroupListView->RequestListRefresh();
}


FReply SSceneMaterialManager::TestSaveData()
{
	DelegateManager::Get()->SaveGameData.Broadcast();
	return FReply::Handled();
}

FReply SSceneMaterialManager::TestReadData()
{
	DelegateManager::Get()->LoadGameData.Broadcast(TEXT("RedPlan"));
	return FReply::Handled();
}

