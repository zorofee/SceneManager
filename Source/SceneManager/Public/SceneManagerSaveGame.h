// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SceneManagerSaveGame.generated.h"



USTRUCT()
struct FMatData {
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;

	UPROPERTY()
		FString parent;

	UPROPERTY()
		FString MatPath;
	
	UPROPERTY()
		TArray<float> ScalarParams;

	UPROPERTY()
		TArray<FVector> VectorParams;
};

USTRUCT()
struct FGroupData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;
	
	UPROPERTY()
		FString parent;

	UPROPERTY()
		TMap<FString,FMatData> MatList;
};

USTRUCT()
struct FPlanData
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;
	
	UPROPERTY()
		TMap<FString,FGroupData> GroupList;
};


/**
 * 
 */
UCLASS()
class SCENEMANAGER_API USceneManagerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FString TestName;

	UPROPERTY()
		TMap<FString,FPlanData> PlanList;



};
