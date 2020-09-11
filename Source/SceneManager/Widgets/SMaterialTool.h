// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelMaterialSettings.h"
#include "Widgets/Colors/SColorBlock.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "DelegateManager.h"

struct FMaterialInfo 
{
public:
	UMaterialInstance* mat;

	FString matName;

	float scalarValue;

	FVector vectorValue;

};


/**
 * 
 */
class SCENEMANAGER_API SMaterialTool : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialTool) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

private:

	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	void OnScalarValueChanged(float value,FMaterialInfo* info);

	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void OnSetColorFromColorPicker(FLinearColor NewColor);


	void AnalysisMaterialParams();

	TSharedRef<SHorizontalBox> GetScalarParamSlot(FMaterialInfo* info);

private:
	FLinearColor CachedColor = FLinearColor(1.0, 0.1, 0.1, 1.0);

	TSharedPtr<SImage> ColorImage;

	TSharedPtr<SVerticalBox> ParamContainer;

	UPROPERTY()
		ULevelMaterialSettings* lms;

	UPROPERTY()
		TMap<FString, FMaterialInfo*> ScalarParamsPair;

	UPROPERTY()
		TMap<FString, FVector> VectorParamsPair;


};
