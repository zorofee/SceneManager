// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SMaterialGroupItemWidget.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */


struct FMaterialGroup
{
	FMaterialGroup() {}

public:
	FString GroupName;

	FString Parent;
};


class SMaterialGroupEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupEntry) {}

	/** Highlight this text in the text block */
	SLATE_ATTRIBUTE(FText, HighlightText)

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroup>& GroupInfo);
};


class SCENEMANAGER_API SMaterialGroupWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroup>& GroupInfo);

private:
	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FMaterialGroupItem>>> ListView;

	/** Array of filtered items to show in the list view */
	TArray<TSharedPtr<FMaterialGroupItem>> FilteredItems;

	/** Generates a widget for the specified item */
	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupItem> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	FReply OnAddGroupItemButtonClicked();


public:
	FMaterialGroup m_GroupInfo;  //parent plan name;

};
