// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LevelPostProcessSettings.h"
#include "DataStructures.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SceneManagerSaveGame.generated.h"



/**
 * 
 */
UCLASS()
class SCENEMANAGER_API USceneManagerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FString DefaultPlan;

	UPROPERTY()
		TMap<FString, FMaterialPlanInfo> PlanList;


	UPROPERTY(EditAnywhere, Category = "PostProcess")
		FPostProcessSettings postprocess;

};


