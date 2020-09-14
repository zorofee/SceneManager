// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


DECLARE_MULTICAST_DELEGATE_ThreeParams(OnMatScalarValueChanged, UMaterialInstance*,FString, float)
DECLARE_MULTICAST_DELEGATE_OneParam(OnAddMatGroup, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(OnAddMatGroupItem, FString)

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
	OnMatScalarValueChanged OnMatScalarValueChanged;

	OnAddMatGroup OnAddMatGroup;

	OnAddMatGroupItem OnAddMatGroupItem;
};
