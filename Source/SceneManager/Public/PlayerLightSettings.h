// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerLightSettings.generated.h"



/**
 * 
 */
UCLASS(MinimalAPI)
class UPlayerLightSettings : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "LightParams")
		FRotator rotation;

	UPROPERTY(EditAnywhere, Category = "LightParams")
		float intensity;

	UPROPERTY(EditAnywhere, Category = "LightParams")
		FLinearColor color;


};
