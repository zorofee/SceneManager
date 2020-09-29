// Fill out your copyright notice in the Description page of Project Settings.


#include "SSettingManager.h"
#include "IDetailsView.h"           // class FDetailsViewArgs.
#include "PropertyEditorModule.h"   // class FPropertyEditorModule.
#include "SaveDataManager.h"

void SSettingManager::Construct(const FArguments& InArgs)
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> SettingView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SettingView->SetObject(SaveDataManager::Get()->pluginSaveSettings);
//	SettingView->OnFinishedChangingProperties().AddRaw(this, &SSettingManager::OnFinishedChangingProperties);

	ChildSlot[
		SettingView
	];
}

