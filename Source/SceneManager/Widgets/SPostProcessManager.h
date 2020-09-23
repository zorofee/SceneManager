// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/PostProcessVolume.h"
#include "LevelPostProcessSettings.h"
#include "Widgets/SWidget.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SPostProcessManager : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPostProcessManager) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

//private:
	//class ULevelPostProcessSettings* m_pps;

public:
	void RefreshContentList(/*FPostProcessSettings& PPS*/);

	void OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent);

	ULevelPostProcessSettings* Setting;

	void GetScenePostProcessVolume();

	void SetScenePostProcessVolume();

	void GetPostProcessParams(APostProcessVolume& Volume);

	void SetPostProcessParams(APostProcessVolume& Volume);

};
