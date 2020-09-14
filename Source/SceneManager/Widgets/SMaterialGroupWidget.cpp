// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupWidget.h"

void SMaterialGroupEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroup>& InItem)
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


void SMaterialGroupWidget::Construct(const FArguments& InArgs)
{
	//TEST ITEM
	//TSharedPtr<FMaterialGroupItem> item1 = MakeShareable(new FMaterialGroupItem());
	//FilteredItems.Emplace(item1);

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		[
			SNew(SVerticalBox)

			//material group list item
			+ SVerticalBox::Slot()
			[
				SAssignNew(ListView, SListView<TSharedPtr<FMaterialGroupItem>>)
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
					.Text(FText::FromString(TEXT("+")))
				]
			]

		]
	];
}

TSharedRef<ITableRow> SMaterialGroupWidget::OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialGroupItem>>, OwnerTable)
		[
			SNew(SMaterialGroupItemEntry, InItem.ToSharedRef())
			//.HighlightText(this, &SSceneManagerTools::GetHighlightText)
		];
}

FReply SMaterialGroupWidget::OnAddGroupItemButtonClicked()
{
	TSharedPtr<FMaterialGroupItem> newItem = MakeShareable(new FMaterialGroupItem());
	FilteredItems.Emplace(newItem);
	ListView->RequestListRefresh();
	return FReply::Handled();
}