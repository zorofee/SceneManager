// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Scene.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelPostProcessSettings.generated.h"


/**
 * 
 */
UCLASS()
class SCENEMANAGER_API ULevelPostProcessSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "PostProcess")
	FPostProcessSettings Setting;
};
