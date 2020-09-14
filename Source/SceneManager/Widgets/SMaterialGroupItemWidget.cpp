// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupItemWidget.h"
#include "SMaterialTool.h"


void SMaterialGroupItemEntry::Construct(const FArguments& InArgs, const TSharedPtr<const FMaterialGroupItem>& InItem)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SMaterialGroupItemWidget)
		]
	];
}

void SMaterialGroupItemWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SMaterialTool)
	];
}