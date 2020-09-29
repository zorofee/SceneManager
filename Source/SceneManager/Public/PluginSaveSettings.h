

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PluginSaveSettings.generated.h"

/**
 * 
 */
UCLASS()
class SCENEMANAGER_API UPluginSaveSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "材质球", meta = (DisplayName = "材质分组名称"))
		TArray<FString> MatGroupNameList;

	UPROPERTY(EditAnywhere, Category = "全局参数", meta = (DisplayName = "角色灯光全局参数"))
		FString PlayerLightMPC;

	UPROPERTY(EditAnywhere, Category = "全局参数", meta = (DisplayName = "场景灯光全局参数"))
		FString SceneLightMPC;
};
