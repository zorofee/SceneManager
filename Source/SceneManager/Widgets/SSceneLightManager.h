// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Materials/MaterialParameterCollection.h"
#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SSceneLightManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSceneLightManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);


};
