// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/Layout/SUniformGridPanel.h"
#include "DataStructures.h"
#include "LevelMaterialSettings.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"




class SMaterialGroupItemEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemEntry) {}

	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialInfo>& InItem);
};



class SCENEMANAGER_API SMaterialGroupItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialInfo>& MatInfo);

	void ChangeSelectedMatInstance();

	void SelectedRepeatedMatInstance();

private:

	void SaveMaterialInfo(const TSharedPtr<const FMaterialInfo>& MatInfo);

	/*
		Widget call back events
	*/

	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	void OnScalarValueChanged(float value, FString name);

	void OnScalarValueCommitted(float NewEqualValue, ETextCommit::Type CommitType, FString name);

	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name,FLinearColor defaultColor, int32 ColorImageIndex);

	void OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window);

	void OnSetColorFromColorPicker(FLinearColor NewColor, FString name, int32 ColorImageIndex);

	TSharedRef<SHorizontalBox> GetScalarParamSlot(FString name, float value);

	TSharedRef<SHorizontalBox> GetVectorParamSlot(FString name, FLinearColor value);


	void LoadMaterialInstanceByInfo();

	void AnalysisMaterialParamsAndPath();

	void ResetMaterialInstanceConstant();

	void AddParamsToSlot();

	void SetMaterialInstanceScalarParam(FString name, float scalar);

	void SetMaterialInstanceVectorParam(FString name, FLinearColor color);

	void SaveMaterialInstance();


private:

	TArray<TSharedPtr<SImage>>  ColorImageArray;

	TSharedPtr<SUniformGridPanel> ParamContainer;

	ULevelMaterialSettings* lms;

	TSharedPtr<FMaterialInfo> m_MaterialInfo;

	FReply OnTest();

	TSet<FString> GroupNameSet;

};
