// Fill out your copyright notice in the Description page of Project Settings.


#include "DelegateManager.h"


DelegateManager::DelegateManager()
{

}

DelegateManager* DelegateManager::instance = new DelegateManager();

DelegateManager* DelegateManager::Get() {
	return instance;
}

