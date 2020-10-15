// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/Light.h"
#include "PlayerLightSettings.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SMPCManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMPCManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs,const FString MPCPath = TEXT(""));

private:
	UMaterialParameterCollection* MPC;
	UMaterialParameterCollectionInstance* MPCIns;
	TSharedPtr<SVerticalBox> ParamsContainer;
	TArray<TSharedPtr<SImage>>  ColorImageArray;

private:
	void SaveMPCParams();
	void RefreshMPCWidgets();
	FReply OnClickColorBlock(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, FString name, FLinearColor defaultColor, int32 ColorImageIndex);
	void OnColorPickerWindowClosed(const TSharedRef<SWindow>& Window);
	void OnSetColorFromColorPicker(FLinearColor NewColor, FString name, int32 ColorImageIndex);
	void AddScalarParam(const FString name, float value);
	void AddVectorParam(const FString name, FLinearColor value, int32 index);
	void OnScalarValueChanged(float value, const FString name);
	void OnScalarValueCommited(float NewValue, ETextCommit::Type CommitType);
	void OnVectorValueChanged(FLinearColor value, const FString name);

private:
	FString TempMPCPath;
};
