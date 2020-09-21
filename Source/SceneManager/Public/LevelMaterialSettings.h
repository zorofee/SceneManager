// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelMaterialSettings.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class ULevelMaterialSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "MatIns")
		UMaterialInstance* material;
};
