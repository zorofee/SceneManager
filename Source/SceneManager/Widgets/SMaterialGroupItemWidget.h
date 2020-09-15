// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelMaterialSettings.h"
#include "Widgets/Colors/SColorBlock.h"
#include "DelegateManager.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


struct FMaterialGroupItem
{
public:
	FString ParentPlan;
	FString ParentGroup;
	FString MatPath;
};


struct FMaterialInfo
{
public:

	UMaterialInstance* mat;

	FString paramName;

	float scalarValue;

	FLinearColor vectorValue;

};


class SMaterialGroupItemEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemEntry) {}

	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem);


};




class SCENEMANAGER_API SMaterialGroupItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem);

private:
	/*
		Widget call back events
	*/


	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	void OnScalarValueChanged(float value, FMaterialInfo* info);

	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void OnSetColorFromColorPicker(FLinearColor NewColor);



	void LoadCurrentMaterial(const FString path);

	void AnalysisMaterialParams();

	void AddParamsToSlot();

	FReply TestSaveData();


	TSharedRef<SHorizontalBox> GetScalarParamSlot(FMaterialInfo* info);

	TSharedRef<SHorizontalBox> GetVectorParamSlot(FMaterialInfo* info);



private:
	FLinearColor CachedColor = FLinearColor(1.0, 0.1, 0.1, 1.0);

	TSharedPtr<SImage> ColorImage;

	TSharedPtr<SVerticalBox> ParamContainer;

	UPROPERTY()
		ULevelMaterialSettings* lms;

	UPROPERTY()
		TMap<FString, FMaterialInfo*> ScalarParamsPair;

	UPROPERTY()
		TMap<FString, FMaterialInfo*> VectorParamsPair;




	FMaterialGroupItem m_GroupItemInfo;

};
