// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures.h"
#include "Widgets/SCompoundWidget.h"
#include "Layout/Visibility.h"
#include "Widgets/SWidget.h"
#include "CoreMinimal.h"
#include "SMaterialGroupWidget.h"

/**
 * 
 */

struct SceneCategoryInfo
{
	/** This category's display name */
	FText DisplayName;

	/** A unique name for this category */
	FName UniqueHandle;

	/** Sort order for the category tab (lowest first) */
	int32 SortOrder;

	/** Optional tag meta data for the tab widget */
	FString TagMetaData;
};





class SSceneManagerTools : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneManagerTools) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	TSharedPtr<SSearchBox> SearchBoxPtr;
	TSharedPtr<SBox> CustomContent;
	TSharedPtr<SBox> LevelMaterialContent;
	TSharedPtr<SBox> LevelLightContent;
	FName ActiveTabName;
	bool bNeedsUpdate;


	TArray<SceneCategoryInfo> Categories;


	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FMaterialGroupInfo>>> ListView;

	/** Array of filtered items to show in the list view */
	TArray<TSharedPtr<FMaterialGroupInfo>> MatGroupItems;

	TSharedPtr<SEditableTextBox> GroupNameText;

	/** Generates a widget for the specified item */
	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable);

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	/** Called when the search text changes */
	void OnSearchChanged(const FText& InFilterText);
	void OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType);

private:
	EVisibility GetTabsVisibility() const;

private:
	/** Creates a tab widget to show on the left that when clicked sets the currently active tab */
	TSharedRef< SWidget > CreatePlacementGroupTab(const SceneCategoryInfo& Info);



	/** Gets the tab 'active' state, so that we can show the active style */
	ECheckBoxState GetPlacementTabCheckedState(FName CategoryName) const;
	/** When the tab is clicked we adjust the check state, so that the right style is displayed. */
	void OnPlacementTabChanged(ECheckBoxState NewState, FName CategoryName);


	/** Gets the border image for the tab, this is the 'active' orange bar. */
	const FSlateBrush* PlacementGroupBorderImage(FName CategoryName) const;

	void UpdateCategoryContent();

	FReply OnAddGroupNameButtonClicked();
	
	FReply TestSaveData();

	FReply TestReadData();
	

public:
	void AddMaterialGroup(const FMaterialGroupInfo& groupInfo);


};