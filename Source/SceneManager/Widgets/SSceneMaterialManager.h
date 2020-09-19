// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataStructures.h"
#include "Widgets/SCompoundWidget.h"
#include "Layout/Visibility.h"
#include "Widgets/SWidget.h"
#include "SMaterialGroupWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SSceneMaterialManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneMaterialManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);


	/*
		Tabs
	*/
private:
	TSharedPtr<SBox> SceneMaterialContent;

	/*
		mat group widgets and events
	*/
private:
	TSharedPtr<SEditableTextBox> GroupNameText;
	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FMaterialGroupInfo> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	FReply AddNewMaterialGroup();
public:
	TSharedPtr<SListView<TSharedPtr<FMaterialGroupInfo>>> MatGroupListView;
	TArray<TSharedPtr<FMaterialGroupInfo>> MatGroupItems;
	void AddMaterialGroup(const FMaterialGroupInfo& groupInfo);
	void DeleteMaterialGroup(TSharedPtr<FMaterialGroupInfo> matInfo);
	void ClearMaterialGroup();


	/*
		ComboBox widgets and events
	*/
private:
	TSharedPtr<SEditableTextBox> PlanNameText;
	TSharedPtr<STextBlock> ComboBoxSelectedText;
	TArray<TSharedPtr<FString>> SourceComboList;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> PlanComboBox;

	FReply OnDeleteCurrentPlan();
	FReply OnAddPlanNameButtonClicked();
	TSharedRef<SWidget> GenerateSourceComboItem(TSharedPtr<FString> InItem);
	void HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
public:
	void SetSelectedPlanName(const FString planName);
	void ResetPlanComboBox(const TArray<FString> planNameList);


	/*
		test
	*/
	FReply TestSaveData();
	FReply TestReadData();
};
