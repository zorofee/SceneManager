// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Widgets/SSceneManagerEntry.h"
#include "DataStructures.h"
#include "SceneManagerSaveGame.h"
#include "CoreMinimal.h"
//#include "MaterialIntermediate.generated.h"

/**
 * 
 */
class  MaterialIntermediate 
{
public:

	TSharedPtr<SSceneManagerTools> SceneManagerTools;


	void LoadSceneManagerTools();

	void PopupMsgDialog(const FString content);

public:
	void AddEventListener();


	void AddMatPlan(FString planName);
	void DeleteMatPlan(FString planName);
	void SelectMatPlan(const FString planName);


	void AddMatGroup(FString groupName);
	void DeleteMatGroup(TSharedPtr<FMaterialGroupInfo> matInfo);


	void AddMatInstance(TSharedPtr<FMaterialInfo> matInfo);
	void DeleteMatInstance(TSharedPtr<FMaterialInfo> matInfo);
	void ReplaceMatInstance(TSharedPtr<FMaterialInfo> matInfo, FString originPath);


	void SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo);

	void SelectMaterialInstance(FString newPath, TSharedPtr<FMaterialInfo> matInfo,  SMaterialGroupItemWidget* widget);


public:
	//post process
	void AddPostProcessPlan(const FString planName);


};
