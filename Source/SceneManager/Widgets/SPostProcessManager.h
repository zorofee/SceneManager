// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/PostProcessVolume.h"
#include "LevelPostProcessSettings.h"
#include "Widgets/SWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SPostProcessManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPostProcessManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

public:
	void RefreshContentList();

	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	void GetScenePostProcessVolume();

	void GetPostProcessParams(FPostProcessSettings setting);

	void GetPostProcessParams(APostProcessVolume& Volume);

	void SetPostProcessParams(APostProcessVolume& Volume);

	FReply OnAddPlan();
private:
	//combobox
	TSharedPtr<SComboBox<TSharedPtr<FString>>> PostProcessComboBox;
	TArray<TSharedPtr<FString>> SourceComboList;
	TSharedPtr<STextBlock> ComboBoxSelectedText;

	TSharedRef<SWidget> GenerateSourceComboItem(TSharedPtr<FString> InItem);
	void HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	void RefreshPostProcessComboList();
	void OnComboBoxOpening();

private:
	
	APostProcessVolume* SelectedPostProcess;

};
