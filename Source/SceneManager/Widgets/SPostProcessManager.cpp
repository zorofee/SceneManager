// Fill out your copyright notice in the Description page of Project Settings.


#include "SPostProcessManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Scene.h"
#include "Widgets/Input/SComboBox.h"
#include "SaveDataManager.h"

void SPostProcessManager::Construct(const FArguments& InArgs)
{
}

void SPostProcessManager::RefreshContentList()
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> PostProcessView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	PostProcessView->OnFinishedChangingProperties().AddRaw(this, &SPostProcessManager::OnFinishedChangingProperties);
	

	PostProcessView->SetObject(SaveDataManager::Get()->Setting);

	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Tab.Text")
				.Text(FText::FromString(TEXT("选择后处理体积:")))
			]

			+ SHorizontalBox::Slot()
			.Padding(10, 0, 0, 0)
			[
				SAssignNew(PostProcessComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&SourceComboList)
				.OnGenerateWidget(this, &SPostProcessManager::GenerateSourceComboItem)
				.OnSelectionChanged(this, &SPostProcessManager::HandleSourceComboChanged)
				.OnComboBoxOpening(this, &SPostProcessManager::OnComboBoxOpening)
				[
					SAssignNew(ComboBoxSelectedText, STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT("Default"))))
				]
			]
		]

		+SVerticalBox::Slot()
		[
			PostProcessView
		]
	];
}


void SPostProcessManager::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	if (SelectedPostProcess != nullptr)
	{
		SetPostProcessParams(*SelectedPostProcess);
	}
}

void SPostProcessManager::GetScenePostProcessVolume()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Actor num is %d"), actorList.Num());
	
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(actorList[i]);
		FString ppName = PostProcessVolume->GetName();
		FString selectedName = *PostProcessComboBox->GetSelectedItem().Get();
		if (ppName.Equals(selectedName))
		{
			SelectedPostProcess = PostProcessVolume;
			GetPostProcessParams(*PostProcessVolume);
		}
	}
}



void SPostProcessManager::SetPostProcessParams(APostProcessVolume& Volume)
{
	if (!Volume.IsValidLowLevel())
	{
		UE_LOG(LogTemp,Warning,TEXT("Post process is not valid"));
		return;
	}

	
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());

	
	//bloom
	Volume.Settings.BloomMethod = SaveDataManager::Get()->Setting->BloomMethod;
	Volume.Settings.BloomIntensity = SaveDataManager::Get()->Setting->BloomIntensity;
	Volume.Settings.BloomThreshold =SaveDataManager::Get()->Setting->BloomThreshold;
	Volume.Settings.BloomSizeScale = SaveDataManager::Get()->Setting->BloomSizeScale;

	Volume.Settings.bOverride_BloomMethod = SaveDataManager::Get()->Setting->bOverride_BloomMethod;
	Volume.Settings.bOverride_BloomIntensity = SaveDataManager::Get()->Setting->bOverride_BloomIntensity;
	Volume.Settings.bOverride_BloomThreshold = SaveDataManager::Get()->Setting->bOverride_BloomThreshold;
	Volume.Settings.bOverride_BloomSizeScale = SaveDataManager::Get()->Setting->bOverride_BloomSizeScale;

	//exposure
	Volume.Settings.AutoExposureMethod = SaveDataManager::Get()->Setting->AutoExposureMethod;
	Volume.Settings.AutoExposureBias = SaveDataManager::Get()->Setting->AutoExposureBias;
	Volume.Settings.AutoExposureApplyPhysicalCameraExposure = SaveDataManager::Get()->Setting->AutoExposureApplyPhysicalCameraExposure;
	Volume.Settings.AutoExposureBiasCurve = SaveDataManager::Get()->Setting->AutoExposureBiasCurve;
	Volume.Settings.AutoExposureMeterMask = SaveDataManager::Get()->Setting->AutoExposureMeterMask;
	Volume.Settings.AutoExposureLowPercent = SaveDataManager::Get()->Setting->AutoExposureLowPercent;
	Volume.Settings.AutoExposureHighPercent = SaveDataManager::Get()->Setting->AutoExposureHighPercent;
	Volume.Settings.AutoExposureMinBrightness = SaveDataManager::Get()->Setting->AutoExposureMinBrightness;
	Volume.Settings.AutoExposureMaxBrightness = SaveDataManager::Get()->Setting->AutoExposureMaxBrightness;
	Volume.Settings.AutoExposureSpeedUp = SaveDataManager::Get()->Setting->AutoExposureSpeedUp;
	Volume.Settings.AutoExposureSpeedDown = SaveDataManager::Get()->Setting->AutoExposureSpeedDown;
	Volume.Settings.HistogramLogMin = SaveDataManager::Get()->Setting->HistogramLogMin;
	Volume.Settings.HistogramLogMax = SaveDataManager::Get()->Setting->HistogramLogMax;

	Volume.Settings.bOverride_AutoExposureMethod = SaveDataManager::Get()->Setting->bOverride_AutoExposureMethod;
	Volume.Settings.bOverride_AutoExposureBias = SaveDataManager::Get()->Setting->bOverride_AutoExposureBias;
	Volume.Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = SaveDataManager::Get()->Setting->bOverride_AutoExposureApplyPhysicalCameraExposure;
	Volume.Settings.bOverride_AutoExposureBiasCurve = SaveDataManager::Get()->Setting->bOverride_AutoExposureBiasCurve;
	Volume.Settings.bOverride_AutoExposureMeterMask = SaveDataManager::Get()->Setting->bOverride_AutoExposureMeterMask;
	Volume.Settings.bOverride_AutoExposureLowPercent = SaveDataManager::Get()->Setting->bOverride_AutoExposureLowPercent;
	Volume.Settings.bOverride_AutoExposureHighPercent = SaveDataManager::Get()->Setting->bOverride_AutoExposureHighPercent;
	Volume.Settings.bOverride_AutoExposureMinBrightness = SaveDataManager::Get()->Setting->bOverride_AutoExposureMinBrightness;
	Volume.Settings.bOverride_AutoExposureMaxBrightness = SaveDataManager::Get()->Setting->bOverride_AutoExposureMaxBrightness;
	Volume.Settings.bOverride_AutoExposureSpeedUp = SaveDataManager::Get()->Setting->bOverride_AutoExposureSpeedUp;
	Volume.Settings.bOverride_AutoExposureSpeedDown = SaveDataManager::Get()->Setting->bOverride_AutoExposureSpeedDown;
	Volume.Settings.bOverride_HistogramLogMin = SaveDataManager::Get()->Setting->bOverride_HistogramLogMin;
	Volume.Settings.bOverride_HistogramLogMax = SaveDataManager::Get()->Setting->bOverride_HistogramLogMax;


	//film
	Volume.Settings.FilmSlope = SaveDataManager::Get()->Setting->FilmSlope;
	Volume.Settings.FilmToe = SaveDataManager::Get()->Setting->FilmToe;
	Volume.Settings.FilmShoulder = SaveDataManager::Get()->Setting->FilmShoulder;
	Volume.Settings.FilmBlackClip = SaveDataManager::Get()->Setting->FilmBlackClip;
	Volume.Settings.FilmWhiteClip = SaveDataManager::Get()->Setting->FilmWhiteClip;

	Volume.Settings.bOverride_FilmSlope = SaveDataManager::Get()->Setting->bOverride_FilmSlope;
	Volume.Settings.bOverride_FilmToe = SaveDataManager::Get()->Setting->bOverride_FilmToe;
	Volume.Settings.bOverride_FilmShoulder = SaveDataManager::Get()->Setting->bOverride_FilmShoulder;
	Volume.Settings.bOverride_FilmBlackClip = SaveDataManager::Get()->Setting->bOverride_FilmBlackClip;
	Volume.Settings.bOverride_FilmWhiteClip = SaveDataManager::Get()->Setting->bOverride_FilmWhiteClip;



	//Rendering Features
	Volume.Settings.AmbientOcclusionIntensity = SaveDataManager::Get()->Setting->AmbientOcclusionIntensity;
	Volume.Settings.AmbientOcclusionRadius = SaveDataManager::Get()->Setting->AmbientOcclusionRadius;
	Volume.Settings.AmbientOcclusionStaticFraction = SaveDataManager::Get()->Setting->AmbientOcclusionStaticFraction;
	Volume.Settings.AmbientOcclusionRadiusInWS = SaveDataManager::Get()->Setting->AmbientOcclusionRadiusInWS;
	Volume.Settings.AmbientOcclusionFadeDistance = SaveDataManager::Get()->Setting->AmbientOcclusionFadeDistance;
	Volume.Settings.AmbientOcclusionFadeRadius = SaveDataManager::Get()->Setting->AmbientOcclusionFadeRadius;
	Volume.Settings.AmbientOcclusionPower = SaveDataManager::Get()->Setting->AmbientOcclusionPower;
	Volume.Settings.AmbientOcclusionBias = SaveDataManager::Get()->Setting->AmbientOcclusionBias;
	Volume.Settings.AmbientOcclusionQuality = SaveDataManager::Get()->Setting->AmbientOcclusionQuality;
	Volume.Settings.AmbientOcclusionMipBlend = SaveDataManager::Get()->Setting->AmbientOcclusionMipBlend;
	Volume.Settings.AmbientOcclusionMipScale = SaveDataManager::Get()->Setting->AmbientOcclusionMipScale;
	Volume.Settings.AmbientOcclusionMipThreshold = SaveDataManager::Get()->Setting->AmbientOcclusionMipThreshold;
	Volume.Settings.AmbientOcclusionOffsetScale = SaveDataManager::Get()->Setting->AmbientOcclusionOffsetScale;
	Volume.Settings.AmbientOcclusionEdgeHighlight = SaveDataManager::Get()->Setting->AmbientOcclusionEdgeHighlight;

	Volume.Settings.bOverride_AmbientOcclusionIntensity = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionIntensity;
	Volume.Settings.bOverride_AmbientOcclusionRadius = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionRadius;
	Volume.Settings.bOverride_AmbientOcclusionStaticFraction = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionStaticFraction;
	Volume.Settings.bOverride_AmbientOcclusionRadiusInWS = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionRadiusInWS;
	Volume.Settings.bOverride_AmbientOcclusionFadeDistance = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionFadeDistance;
	Volume.Settings.bOverride_AmbientOcclusionFadeRadius = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionFadeRadius;
	Volume.Settings.bOverride_AmbientOcclusionPower = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionPower;
	Volume.Settings.bOverride_AmbientOcclusionBias = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionBias;
	Volume.Settings.bOverride_AmbientOcclusionQuality = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionQuality;
	Volume.Settings.bOverride_AmbientOcclusionMipBlend = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipBlend;

	Volume.Settings.bOverride_AmbientOcclusionMipScale = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipScale;
	Volume.Settings.bOverride_AmbientOcclusionMipThreshold = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipThreshold;
	Volume.Settings.bOverride_AmbientOcclusionOffsetScale = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionOffsetScale;
	Volume.Settings.bOverride_AmbientOcclusionEdgeHighlight = SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionEdgeHighlight;
}


void SPostProcessManager::GetPostProcessParams(APostProcessVolume& Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());

	//bloom
	SaveDataManager::Get()->Setting->BloomMethod = Volume.Settings.BloomMethod;
	SaveDataManager::Get()->Setting->BloomIntensity = Volume.Settings.BloomIntensity;
	SaveDataManager::Get()->Setting->BloomThreshold = Volume.Settings.BloomThreshold;
	SaveDataManager::Get()->Setting->BloomSizeScale = Volume.Settings.BloomSizeScale;

	SaveDataManager::Get()->Setting->bOverride_BloomMethod = Volume.Settings.bOverride_BloomMethod ;
	SaveDataManager::Get()->Setting->bOverride_BloomIntensity = Volume.Settings.bOverride_BloomIntensity  ;
	SaveDataManager::Get()->Setting->bOverride_BloomThreshold = Volume.Settings.bOverride_BloomThreshold  ;
	SaveDataManager::Get()->Setting->bOverride_BloomSizeScale = Volume.Settings.bOverride_BloomSizeScale  ;

	//exposure
	SaveDataManager::Get()->Setting->AutoExposureMethod = Volume.Settings.AutoExposureMethod;
	SaveDataManager::Get()->Setting->AutoExposureBias = Volume.Settings.AutoExposureBias;
	SaveDataManager::Get()->Setting->AutoExposureApplyPhysicalCameraExposure = Volume.Settings.AutoExposureApplyPhysicalCameraExposure ;
	SaveDataManager::Get()->Setting->AutoExposureBiasCurve = Volume.Settings.AutoExposureBiasCurve;
	SaveDataManager::Get()->Setting->AutoExposureMeterMask = Volume.Settings.AutoExposureMeterMask;
	SaveDataManager::Get()->Setting->AutoExposureLowPercent = Volume.Settings.AutoExposureLowPercent;
	SaveDataManager::Get()->Setting->AutoExposureHighPercent = Volume.Settings.AutoExposureHighPercent ;
	SaveDataManager::Get()->Setting->AutoExposureMinBrightness = Volume.Settings.AutoExposureMinBrightness  ;
	SaveDataManager::Get()->Setting->AutoExposureMaxBrightness = Volume.Settings.AutoExposureMaxBrightness  ;
	SaveDataManager::Get()->Setting->AutoExposureSpeedUp = Volume.Settings.AutoExposureSpeedUp  ;
	SaveDataManager::Get()->Setting->AutoExposureSpeedDown = Volume.Settings.AutoExposureSpeedDown  ;
	SaveDataManager::Get()->Setting->HistogramLogMin = Volume.Settings.HistogramLogMin ;
	SaveDataManager::Get()->Setting->HistogramLogMax = Volume.Settings.HistogramLogMax  ;

	SaveDataManager::Get()->Setting->bOverride_AutoExposureMethod = Volume.Settings.bOverride_AutoExposureMethod  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureBias = Volume.Settings.bOverride_AutoExposureBias  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureApplyPhysicalCameraExposure = Volume.Settings.bOverride_AutoExposureApplyPhysicalCameraExposure  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureBiasCurve = Volume.Settings.bOverride_AutoExposureBiasCurve  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureMeterMask = Volume.Settings.bOverride_AutoExposureMeterMask  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureLowPercent = Volume.Settings.bOverride_AutoExposureLowPercent  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureHighPercent = Volume.Settings.bOverride_AutoExposureHighPercent  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureMinBrightness = Volume.Settings.bOverride_AutoExposureMinBrightness  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureMaxBrightness = Volume.Settings.bOverride_AutoExposureMaxBrightness ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureSpeedUp = Volume.Settings.bOverride_AutoExposureSpeedUp  ;
	SaveDataManager::Get()->Setting->bOverride_AutoExposureSpeedDown = Volume.Settings.bOverride_AutoExposureSpeedDown  ;
	SaveDataManager::Get()->Setting->bOverride_HistogramLogMin = Volume.Settings.bOverride_HistogramLogMin  ;
	SaveDataManager::Get()->Setting->bOverride_HistogramLogMax = Volume.Settings.bOverride_HistogramLogMax  ;

	//film
	SaveDataManager::Get()->Setting->FilmSlope = Volume.Settings.FilmSlope  ;
	SaveDataManager::Get()->Setting->FilmToe = Volume.Settings.FilmToe  ;
	SaveDataManager::Get()->Setting->FilmShoulder = Volume.Settings.FilmShoulder  ;
	SaveDataManager::Get()->Setting->FilmBlackClip = Volume.Settings.FilmBlackClip  ;
	SaveDataManager::Get()->Setting->FilmWhiteClip = Volume.Settings.FilmWhiteClip  ;

	SaveDataManager::Get()->Setting->bOverride_FilmSlope = Volume.Settings.bOverride_FilmSlope  ;
	SaveDataManager::Get()->Setting->bOverride_FilmToe = Volume.Settings.bOverride_FilmToe  ;
	SaveDataManager::Get()->Setting->bOverride_FilmShoulder = Volume.Settings.bOverride_FilmShoulder  ;
	SaveDataManager::Get()->Setting->bOverride_FilmBlackClip = Volume.Settings.bOverride_FilmBlackClip  ;
	SaveDataManager::Get()->Setting->bOverride_FilmWhiteClip = Volume.Settings.bOverride_FilmWhiteClip  ;

	//Rendering Features
	SaveDataManager::Get()->Setting->AmbientOcclusionIntensity = Volume.Settings.AmbientOcclusionIntensity  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionRadius = Volume.Settings.AmbientOcclusionRadius  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionStaticFraction = Volume.Settings.AmbientOcclusionStaticFraction  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionRadiusInWS = Volume.Settings.AmbientOcclusionRadiusInWS  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionFadeDistance = Volume.Settings.AmbientOcclusionFadeDistance  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionFadeRadius = Volume.Settings.AmbientOcclusionFadeRadius  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionPower = Volume.Settings.AmbientOcclusionPower ;
	SaveDataManager::Get()->Setting->AmbientOcclusionBias = Volume.Settings.AmbientOcclusionBias ;
	SaveDataManager::Get()->Setting->AmbientOcclusionQuality = Volume.Settings.AmbientOcclusionQuality  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionMipBlend = Volume.Settings.AmbientOcclusionMipBlend ;
	SaveDataManager::Get()->Setting->AmbientOcclusionMipScale = Volume.Settings.AmbientOcclusionMipScale  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionMipThreshold = Volume.Settings.AmbientOcclusionMipThreshold  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionOffsetScale = Volume.Settings.AmbientOcclusionOffsetScale  ;
	SaveDataManager::Get()->Setting->AmbientOcclusionEdgeHighlight = Volume.Settings.AmbientOcclusionEdgeHighlight  ;

	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionIntensity = Volume.Settings.bOverride_AmbientOcclusionIntensity  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionRadius = Volume.Settings.bOverride_AmbientOcclusionRadius  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionStaticFraction = Volume.Settings.bOverride_AmbientOcclusionStaticFraction ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionRadiusInWS = Volume.Settings.bOverride_AmbientOcclusionRadiusInWS  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionFadeDistance = Volume.Settings.bOverride_AmbientOcclusionFadeDistance  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionFadeRadius = Volume.Settings.bOverride_AmbientOcclusionFadeRadius  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionPower = Volume.Settings.bOverride_AmbientOcclusionPower  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionBias = Volume.Settings.bOverride_AmbientOcclusionBias  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionQuality = Volume.Settings.bOverride_AmbientOcclusionQuality  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipBlend = Volume.Settings.bOverride_AmbientOcclusionMipBlend  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipScale = Volume.Settings.bOverride_AmbientOcclusionMipScale ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionMipThreshold = Volume.Settings.bOverride_AmbientOcclusionMipThreshold ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionOffsetScale = Volume.Settings.bOverride_AmbientOcclusionOffsetScale  ;
	SaveDataManager::Get()->Setting->bOverride_AmbientOcclusionEdgeHighlight = Volume.Settings.bOverride_AmbientOcclusionEdgeHighlight  ;
}




TSharedRef<SWidget> SPostProcessManager::GenerateSourceComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}


void SPostProcessManager::HandleSourceComboChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item.IsValid())
	{
		PostProcessComboBox->SetSelectedItem(Item);
		ComboBoxSelectedText->SetText(FText::FromString(*Item.Get()));

		GetScenePostProcessVolume();
	}
}

void SPostProcessManager::OnComboBoxOpening()
{
	SourceComboList.Empty();
	RefreshPostProcessComboList();
	PostProcessComboBox->RefreshOptions();
}

void SPostProcessManager::RefreshPostProcessComboList()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Postprocess num is %d"), actorList.Num());
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		SourceComboList.Emplace(MakeShareable(new FString(actorList[i]->GetName())));
	}
}
