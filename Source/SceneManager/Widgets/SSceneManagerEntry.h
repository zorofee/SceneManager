// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures.h"
#include "SSceneMaterialManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Layout/Visibility.h"
#include "Widgets/SWidget.h"
#include "SMaterialGroupWidget.h"
#include "CoreMinimal.h"
#include "Widgets/Input/SComboBox.h"


class SSceneManagerTools : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneManagerTools) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);



private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;


public:
	TSharedPtr<SSceneMaterialManager> SceneMaterialManager;
	/*
		Tabs
	*/
private:
	TSharedPtr<SSearchBox> SearchBoxPtr;
	TSharedPtr<SBox> CustomContent;
	TSharedPtr<SBox> LevelLightContent;
	FName ActiveTabName;
	bool bNeedsUpdate;
	TArray<SceneCategoryInfo> Categories;
	TSharedRef< SWidget > CreatePlacementGroupTab(const SceneCategoryInfo& Info);
	ECheckBoxState GetPlacementTabCheckedState(FName CategoryName) const;
	void OnPlacementTabChanged(ECheckBoxState NewState, FName CategoryName);
	const FSlateBrush* PlacementGroupBorderImage(FName CategoryName) const;
	EVisibility GetTabsVisibility() const;




};