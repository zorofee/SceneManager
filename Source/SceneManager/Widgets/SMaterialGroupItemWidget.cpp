// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialGroupItemWidget.h"
#include "SMaterialTool.h"

void SMaterialGroupItemWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SMaterialTool)
		];
}