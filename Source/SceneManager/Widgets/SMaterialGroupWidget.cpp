// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupWidget.h"
#include "DelegateManager.h"

void SMaterialGroupEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 10, 0, 10)
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
				.Text(FText::FromString(GroupInfo->GroupName))
			]

			+ SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(2, 0, 4, 0)
			[
				SNew(SMaterialGroupWidget, GroupInfo)
			]
		]
	];
}


void SMaterialGroupWidget::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo)
{
	SaveGroupInfo(GroupInfo);

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		.Padding(FMargin(10,5))
		[
			SNew(SVerticalBox)

			//material group list item
			+ SVerticalBox::Slot()
			[
				SAssignNew(ListView, SListView<TSharedPtr<FMaterialInfo>>)
				.ListItemsSource(&FilteredItems)
				.OnGenerateRow(this, &SMaterialGroupWidget::OnGenerateWidgetForItem)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &SMaterialGroupWidget::OnAddGroupItemButtonClicked)
					.Text(FText::FromString(TEXT("+ New Mat")))
					.ContentPadding(4.0f)
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(40,0,0,0)
				[
					SNew(SButton)
					.OnClicked(this, &SMaterialGroupWidget::OnRemoveGroupButtonClicked)
					.Text(FText::FromString(TEXT("Remove Group")))
					.ContentPadding(4.0f)
				]
			]

			
		]
	];


	//如果已有材质列表,则需要刷新
	if (GroupInfo->MatList.Num()>0)
	{
		for (TPair<FString, FMaterialInfo> iterator : GroupInfo->MatList)
		{
			AddMaterialInstanceByInfo(iterator.Value);
		}
	}
}

void SMaterialGroupWidget::SaveGroupInfo(const TSharedPtr<const FMaterialGroupInfo>& GroupInfo)
{
	m_GroupInfo = MakeShareable(new FMaterialGroupInfo());
	m_GroupInfo->GroupName = GroupInfo.Get()->GroupName;
	m_GroupInfo->Parent = GroupInfo.Get()->Parent;
	m_GroupInfo->MatList = GroupInfo.Get()->MatList;
}


TSharedRef<ITableRow> SMaterialGroupWidget::OnGenerateWidgetForItem(TSharedPtr<FMaterialInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialInfo>>, OwnerTable)
		[
			SNew(SMaterialGroupItemEntry, InItem.ToSharedRef())
		];
}

FReply SMaterialGroupWidget::OnAddGroupItemButtonClicked()
{
	TSharedPtr<FMaterialInfo> MatInfo = MakeShareable(new FMaterialInfo());
	MatInfo->ParentGroup = m_GroupInfo->GroupName;
	MatInfo->ParentPlan = m_GroupInfo->Parent;
	FilteredItems.Emplace(MatInfo);
	ListView->RequestListRefresh();
	return FReply::Handled();
}

FReply SMaterialGroupWidget::OnRemoveGroupButtonClicked()
{
	DelegateManager::Get()->DeleteSceneMatGroup.Broadcast(m_GroupInfo);
	return FReply::Handled();
}

void SMaterialGroupWidget::AddMaterialInstanceByInfo(const FMaterialInfo& matInfo)
{
	TSharedPtr<FMaterialInfo> MatInfo = MakeShareable(new FMaterialInfo());
	MatInfo->ParentGroup = matInfo.ParentGroup;
	MatInfo->ParentPlan = matInfo.ParentPlan;
	MatInfo->MatPath = matInfo.MatPath;
	MatInfo->ScalarParams = matInfo.ScalarParams;
	MatInfo->VectorParams = matInfo.VectorParams;
	FilteredItems.Emplace(MatInfo);
	/*ListView->RequestListRefresh();*/
}


void SMaterialGroupWidget::ClearMaterialGroupItems()
{

}
