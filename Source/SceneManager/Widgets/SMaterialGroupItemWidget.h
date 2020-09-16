// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/Layout/SUniformGridPanel.h"
#include "DataStructures.h"
#include "LevelMaterialSettings.h"
#include "Widgets/Colors/SColorBlock.h"
#include "DelegateManager.h"
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

private:

	void SaveMaterialInfo(const TSharedPtr<const FMaterialInfo>& MatInfo);

	/*
		Widget call back events
	*/


	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	void OnScalarValueChanged(float value, FString name);

	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name);

	void OnSetColorFromColorPicker(FLinearColor NewColor, FString name);



	void LoadMaterialInstanceByInfo();

	void AnalysisMaterialParams();

	void AddParamsToSlot();

	FReply DeleteCurrentMat();


	TSharedRef<SHorizontalBox> GetScalarParamSlot(FString name, float value);

	TSharedRef<SHorizontalBox> GetVectorParamSlot(FString name, FLinearColor value);


private:
	FLinearColor CachedColor = FLinearColor(1.0, 0.1, 0.1, 1.0);

	TSharedPtr<SImage> ColorImage;

	TSharedPtr<SUniformGridPanel> ParamContainer;


	ULevelMaterialSettings* lms;

	TSharedPtr<FMaterialInfo> m_MaterialInfo;



};
