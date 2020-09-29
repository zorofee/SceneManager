// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PluginSaveSettings.h"
#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SSettingManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSettingManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

};
