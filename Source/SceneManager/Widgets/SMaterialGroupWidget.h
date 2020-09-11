// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */


struct FMaterialGroupItem
{
	FMaterialGroupItem() {}
};


class SMaterialGroupEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupEntry) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem);

	TSharedPtr<const FMaterialGroupItem> Item;

};


class SCENEMANAGER_API SMaterialGroupWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

private:
	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FMaterialGroupItem>>> ListView;

	/** Array of filtered items to show in the list view */
	TArray<TSharedPtr<FMaterialGroupItem>> FilteredItems;

	/** Generates a widget for the specified item */
	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupItem> InItem, const TSharedRef<STableViewBase>& OwnerTable);
};
