// Fill out your copyright notice in the Description page of Project Settings.


#include "SSceneLightManager.h"


void SSceneLightManager::Construct(const FArguments& InArgs)
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> CollectionView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	CollectionView->OnFinishedChangingProperties().AddRaw(this, &SSceneLightManager::OnFinishedChangingProperties);

	UMaterialParameterCollection* loadasset = Cast<UMaterialParameterCollection>(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL, TEXT("/Game/MPC_Scene.MPC_Scene")));
	if (loadasset != nullptr)
	{
		MPC = loadasset;
	}
	else
	{
		MPC = NewObject<UMaterialParameterCollection>();
	}

	CollectionView->SetObject(MPC);

	ChildSlot
		[
			CollectionView
		];
}

void SSceneLightManager::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{

}
