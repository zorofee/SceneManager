

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
	UPROPERTY(EditAnywhere, Category = "Scene Material", meta = (DisplayName = "材质分组名称"))
		TArray<FString> MatGroupName;

};
