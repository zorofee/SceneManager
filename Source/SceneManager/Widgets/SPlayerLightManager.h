// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/Light.h"
#include "PlayerLightSettings.h"
#include "Materials/MaterialParameterCollection.h"
#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SPlayerLightManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerLightManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

private:
	UMaterialParameterCollection* MPC;
	UPlayerLightSettings* LightSetting;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> LightComboBox;
	TSharedPtr<SEditableTextBox> PlanNameText;
	TSharedPtr<STextBlock> ComboBoxSelectedText;
	TArray<TSharedPtr<FString>> SourceComboList;
	TSharedPtr<SVerticalBox> ParamsContainer;
	TArray<TSharedPtr<SImage>>  ColorImageArray;
	ALight* SelectedLight;

private:
	void OnFinishedChangingMainLight(const FPropertyChangedEvent& InEvent);
	void RefreshLightComboList();
	void RefreshMPCWidgets();
	void FindSceneLight(const FString lightName);
	TSharedRef<SWidget> GenerateSourceComboItem(TSharedPtr<FString> InItem);
	void HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	void OnComboBoxOpening();
	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name, FLinearColor defaultColor, int32 ColorImageIndex);
	void OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window);
	void OnSetColorFromColorPicker(FLinearColor NewColor, FString name, int32 ColorImageIndex);
	void AddScalarParam(const FString name, float value);
	void AddVectorParam(const FString name, FLinearColor value, int32 index);
	void OnScalarValueChanged(float value, const FString name);
	void OnScalarVectorChanged(FLinearColor value, const FString name);
};
