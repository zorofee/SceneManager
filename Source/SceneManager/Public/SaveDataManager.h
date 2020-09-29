

#pragma once
#include "LevelPostProcessSettings.h"
#include "PluginSaveSettings.h"
#include "SceneManagerSaveGame.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SaveDataManager
{
public:
	static SaveDataManager* Get();

	void LoadData();

	void SaveData();

public:
	FMaterialPlanInfo* GetSelectedPlan();

	FMaterialGroupInfo* GetGroup(TSharedPtr<FMaterialInfo> matInfo);

	void AddMatInstanceData(TSharedPtr<FMaterialInfo> matInfo);

	void DeleteMatInstanceData(TSharedPtr<FMaterialInfo> matInfo);

	void SetMatScalarParam(TSharedPtr<FMaterialInfo> matInfo);

	void ReplaceMatInstanceData(TSharedPtr<FMaterialInfo> matInfo, FString originPath);

public:
	void CopyPlanData(const FMaterialPlanInfo* fromPlan, FMaterialPlanInfo* toPlan);

public:
	bool IsMaterialExisted(TSharedPtr<FMaterialInfo> matInfo);

	bool IsMaterialGroupExisted(TSharedPtr<FMaterialInfo> matInfo);

	bool IsMaterialPlanExisted(TSharedPtr<FMaterialInfo> matInfo);


private:
	SaveDataManager();

	static SaveDataManager* instance;

public:
	USceneManagerSaveGame* saveGame;

	UPluginSaveSettings* pluginSaveSettings;

	ULevelPostProcessSettings* Setting;

	bool bAllowCopySelectedPlan;

};
