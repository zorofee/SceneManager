// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../Widgets/SMaterialGroupItemWidget.h"
#include "DataStructures.h"
#include "CoreMinimal.h"


DECLARE_MULTICAST_DELEGATE_OneParam(RefreshPlanListEvent,FString)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatPlanEvent, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatGroupEvent, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(AddSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>)
DECLARE_MULTICAST_DELEGATE_OneParam(OnMatParamChanged, TSharedPtr<FMaterialInfo>)

DECLARE_MULTICAST_DELEGATE_OneParam(DeleteSceneMatPlanEvent, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(DeleteSceneMatGroupEvent, TSharedPtr<FMaterialGroupInfo>)
DECLARE_MULTICAST_DELEGATE_OneParam(DeleteSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>)
DECLARE_MULTICAST_DELEGATE_TwoParams(ReplaceSceneMatInstanceEvent, TSharedPtr<FMaterialInfo>,FString)

DECLARE_MULTICAST_DELEGATE_ThreeParams(SelectMaterialInstanceEvent,FString, TSharedPtr<FMaterialInfo>, SMaterialGroupItemWidget*)

DECLARE_MULTICAST_DELEGATE_OneParam(PopupMsgDialogEvent, const FString)

DECLARE_MULTICAST_DELEGATE_OneParam(AddPostProcessPlanEvent,const FString)


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

	RefreshPlanListEvent RefreshPlanList;

	DeleteSceneMatPlanEvent DeleteSceneMatPlan;

	DeleteSceneMatGroupEvent DeleteSceneMatGroup;

	DeleteSceneMatInstanceEvent DeleteSceneMatInstance;

	ReplaceSceneMatInstanceEvent ReplaceSceneMatInstance;

	SelectMaterialInstanceEvent SelectMaterialInstance;

	PopupMsgDialogEvent PopupMsgDialog;

	AddPostProcessPlanEvent AddPostProcessPlan;
};
