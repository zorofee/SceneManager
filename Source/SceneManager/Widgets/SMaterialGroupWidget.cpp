// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupWidget.h"
#include "SMaterialGroupItemWidget.h"

void SMaterialGroupEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SMaterialGroupItemWidget)
		]
	];
}

void SMaterialGroupWidget::Construct(const FArguments& InArgs)
{

	for (size_t i = 0; i < 5; i++)
	{
		TSharedPtr<FMaterialGroupItem> item1 = MakeShareable(new FMaterialGroupItem());
		FilteredItems.Emplace(item1);
	}

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		[
			SNew(SHorizontalBox)

			//material group list item
			+ SHorizontalBox::Slot()
			[
				SAssignNew(ListView, SListView<TSharedPtr<FMaterialGroupItem>>)
				.ListItemsSource(&FilteredItems)
				.OnGenerateRow(this, &SMaterialGroupWidget::OnGenerateWidgetForItem)
			]
		]
	];
}

TSharedRef<ITableRow> SMaterialGroupWidget::OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FMaterialGroupItem>>, OwnerTable)
		[
			SNew(SMaterialGroupEntry, InItem.ToSharedRef())
			//.HighlightText(this, &SSceneManagerTools::GetHighlightText)
		];
}