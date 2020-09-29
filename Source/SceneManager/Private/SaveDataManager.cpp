


#include "SaveDataManager.h"


SaveDataManager::SaveDataManager()
{

}

SaveDataManager* SaveDataManager::instance = new SaveDataManager();

SaveDataManager* SaveDataManager::Get() {
	return instance;
}

