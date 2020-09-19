// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SMaterialGroupItemWidget.h"
#include "DataStructures.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */

class SMaterialGroupEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupEntry) {}

	/** Highlight this text in the text block */
	SLATE_ATTRIBUTE(FText, HighlightText)

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo);
	//void Construct(const FArguments& InArgs, FMaterialGroupInfo& GroupInfo);
};


class SCENEMANAGER_API SMaterialGroupWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupInfo>& GroupInfo);
	//void Construct(const FArguments& InArgs, FMaterialGroupInfo& GroupInfo);

private:
	/** List view that shows placeable items */
	TSharedPtr<SListView<TSharedPtr<FMaterialInfo>>> ListView;

	/** Array of filtered items to show in the list view */
	TArray<TSharedPtr<FMaterialInfo>> FilteredItems;

	/** Generates a widget for the specified item */
	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FMaterialInfo> InItem,const TSharedRef<STableViewBase>& OwnerTable);

	FReply AddNewMaterialInstance();

	FReply OnRemoveGroupButtonClicked();


	void SaveGroupInfo(const TSharedPtr<const FMaterialGroupInfo>& GroupInfo);


public:
	//FMaterialGroupInfo m_GroupInfo;  //parent plan name;
	TSharedPtr<FMaterialGroupInfo> m_GroupInfo;

	void AddMaterialInstanceByInfo(const FMaterialInfo& matInfo);

	void ClearMaterialGroupItems();


	bool IsHovered;

	bool bIsItemExpanded = true;

	const FSlateBrush* GetExpanderImage() const;

	FReply OnClickExpanderArrow();

};
