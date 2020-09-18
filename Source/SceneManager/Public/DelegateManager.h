// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures.h"
#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(SaveGameDataEvent)
DECLARE_MULTICAST_DELEGATE_OneParam(LoadGameDataEvent, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(RefreshPlanListEvent,FString)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatPlanEvent, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatGroupEvent, TSharedPtr<FMaterialGroupInfo>)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>)
DECLARE_MULTICAST_DELEGATE_OneParam(OnMatParamChanged, TSharedPtr<FMaterialInfo>)

DECLARE_MULTICAST_DELEGATE_OneParam(DeleteSceneMatGroupEvent, TSharedPtr<FMaterialGroupInfo>)
DECLARE_MULTICAST_DELEGATE_OneParam(DeleteSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>)
DECLARE_MULTICAST_DELEGATE_TwoParams(ReplaceSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>,FString)

/**
 * 
 */
class SCENEMANAGER_API DelegateManager
{
public:
	static DelegateManager* Get();

private:
	DelegateManager();

	static DelegateManager* instance;

public:

	AddSceneMatPlanEvent AddSceneMatPlan;
	
	AddSceneMatGroupEvent AddSceneMatGroup;

	AddSceneMatInstanceEvent AddSceneMatInstance;

	OnMatParamChanged OnMatParamChanged;

	SaveGameDataEvent SaveGameData;

	LoadGameDataEvent LoadGameData;

	RefreshPlanListEvent RefreshPlanList;

	DeleteSceneMatGroupEvent DeleteSceneMatGroup;

	DeleteSceneMatInstanceEvent DeleteSceneMatInstance;

	ReplaceSceneMatInstanceEvent ReplaceSceneMatInstance;
};
