// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataStructures.generated.h"


USTRUCT()
struct FMaterialInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString ParentPlan;

	UPROPERTY()
		FString ParentGroup;

	UPROPERTY()
		FString MatPath;

	UPROPERTY()
		TMap<FString, float> ScalarParams;

	UPROPERTY()
		TMap<FString, FLinearColor> VectorParams;
};


USTRUCT()
struct FMaterialGroupInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString GroupName;

	UPROPERTY()
		FString Parent;

	UPROPERTY()
		TMap<FString, FMaterialInfo> MatList;
};



USTRUCT()
struct FMaterialPlanInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString Name;

	UPROPERTY()
		TMap<FString, FMaterialGroupInfo> GroupList;
};


struct SceneCategoryInfo
{
	/** This category's display name */
	FText DisplayName;

	/** A unique name for this category */
	FName UniqueHandle;

	/** Sort order for the category tab (lowest first) */
	int32 SortOrder;

	/** Optional tag meta data for the tab widget */
	FString TagMetaData;
};
