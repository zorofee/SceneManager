// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
/**
 * 
 */

struct FMaterialGroupItem
{
	FMaterialGroupItem() {}
};


class SMaterialGroupItemEntry : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemEntry) {}

	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem);

	TSharedPtr<const FMaterialGroupItem> Item;

};

class SCENEMANAGER_API SMaterialGroupItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialGroupItemWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
};
