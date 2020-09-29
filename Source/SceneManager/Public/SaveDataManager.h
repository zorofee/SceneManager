

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SCENEMANAGER_API SaveDataManager
{
public:
	static SaveDataManager* Get();

private:
	SaveDataManager();

	static SaveDataManager* instance;

};
