// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EPlanListType
{
	Material,
	PostProcess
};

/**
 * 
 */
class SCENEMANAGER_API SPlanDropList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlanDropList) {}
		SLATE_EVENT(FOnClicked, OnAddPlan)
		SLATE_ARGUMENT(EPlanListType, Type)
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	FReply ExecuteOnClick();

	void CheckBoxChanged(ECheckBoxState newState);

public:
	FOnClicked OnAddPlan;
	TEnumAsByte<EPlanListType> Type;
	TSharedPtr<SEditableTextBox> PlanNameText;

};
