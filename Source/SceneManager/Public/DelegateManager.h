// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"




DECLARE_MULTICAST_DELEGATE_TwoParams(OnMatScalarValueChangedEvent, FString, float)
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
	OnMatScalarValueChangedEvent OnMatScalarValueChanged;
};
