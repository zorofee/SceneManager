// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupWidget.h"
#include "DelegateManager.h"
#include "Widgets/Views/SExpanderArrow.h"

void SMaterialGroupEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo)
{
	ChildSlot
	[
		SNew(SMaterialGroupWidget, GroupInfo)
	];
}


void SMaterialGroupWidget::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo)
{
	
	SaveGroupInfo(GroupInfo);

	FString groupName = FString::Printf(TEXT("%s"),  *GroupInfo->GroupName);

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 10, 0, 10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(FCoreStyle::Get(), "NoBorder")
				.OnClicked(this, &SMaterialGroupWidget::OnClickExpanderArrow)
				[
					SNew(SImage)
					.Image(this, &SMaterialGroupWidget::GetExpanderImage)
					.ColorAndOpacity(FSlateColor::UseForeground())
				]
			]
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SAssignNew(GroupNameText,STextBlock)
				.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
				.Text(FText::FromString(*groupName))
			]


			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked(this, &SMaterialGroupWidget::AddNewMaterialInstance)
				.Text(FText::FromString(TEXT("+ 添加材质")))
				.ContentPadding(2.0f)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(25, 0, 0, 0)
			[
				SNew(SButton)
				.OnClicked(this, &SMaterialGroupWidget::OnRemoveGroupButtonClicked)
				.Text(FText::FromString(TEXT("删除分组")))
				.ContentPadding(2.0f)
			]

			
		]

		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(2, 0, 4, 0)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			.Padding(FMargin(20, 10))
			[

				SAssignNew(ListView, SListView<TSharedPtr<FMaterialInfo>>)
				.ListItemsSource(&FilteredItems)
				.OnGenerateRow(this, &SMaterialGroupWidget::OnGenerateWidgetForItem)
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
	else
	{
		//没有材质列表则创建一个默认的
		AddNewMaterialInstance();
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

FReply SMaterialGroupWidget::AddNewMaterialInstance()
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


FReply SMaterialGroupWidget::OnClickExpanderArrow()
{
	ListView->SetVisibility(bIsItemExpanded ? EVisibility::Collapsed : EVisibility::Visible);
	bIsItemExpanded = !bIsItemExpanded;
	return FReply::Handled();
}

const FSlateBrush* SMaterialGroupWidget::GetExpanderImage() const
{
	//const bool bIsItemExpanded = true;

	FName ResourceName;
	if (bIsItemExpanded)
	{
		if (IsHovered)
		{
			static FName ExpandedHoveredName = "TreeArrow_Expanded_Hovered";
			ResourceName = ExpandedHoveredName;
		}
		else
		{
			static FName ExpandedName = "TreeArrow_Expanded";
			ResourceName = ExpandedName;
		}
	}
	else
	{
		if (IsHovered)
		{
			static FName CollapsedHoveredName = "TreeArrow_Collapsed_Hovered";
			ResourceName = CollapsedHoveredName;
		}
		else
		{
			static FName CollapsedName = "TreeArrow_Collapsed";
			ResourceName = CollapsedName;
		}
	}

	ResourceName = "TreeArrow_Expanded";
	return FEditorStyle::GetBrush(ResourceName);
}
