// Fill out your copyright notice in the Description page of Project Settings.


#include "SPostProcessManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Scene.h"

void SPostProcessManager::Construct(const FArguments& InArgs)
{
}

void SPostProcessManager::RefreshContentList(/*FPostProcessSettings& PPS*/)
{
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.bAllowSearch = false;
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	TSharedRef<IDetailsView> PostProcessView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	PostProcessView->OnFinishedChangingProperties().AddRaw(this, &SPostProcessManager::OnFinishedChangingProperties);
	
	Setting = NewObject<ULevelPostProcessSettings>();
	GetScenePostProcessVolume();
	PostProcessView->SetObject(Setting);

	ChildSlot
	[
		PostProcessView
	];
}


void SPostProcessManager::OnFinishedChangingProperties(const FPropertyChangedEvent& InEvent)
{
	SetScenePostProcessVolume();
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
		GetPostProcessParams(*PostProcessVolume);
	}
}


void SPostProcessManager::SetScenePostProcessVolume()
{
	TArray<AActor*> actorList;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	ULevel* Level = World->GetCurrentLevel();

	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), actorList);
	UE_LOG(LogTemp, Warning, TEXT("Actor num is %d"), actorList.Num());
	for (size_t i = 0; i < actorList.Num(); i++)
	{
		APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(actorList[i]);
		SetPostProcessParams(*PostProcessVolume);
	}
}

void SPostProcessManager::SetPostProcessParams(APostProcessVolume& Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());


	//bloom
	Volume.Settings.BloomMethod = Setting->BloomMethod;
	Volume.Settings.BloomIntensity = Setting->BloomIntensity;
	Volume.Settings.BloomThreshold = Setting->BloomThreshold;
	Volume.Settings.BloomSizeScale = Setting->BloomSizeScale;

	Volume.Settings.bOverride_BloomMethod = Setting->bOverride_BloomMethod;
	Volume.Settings.bOverride_BloomIntensity = Setting->bOverride_BloomIntensity;
	Volume.Settings.bOverride_BloomThreshold = Setting->bOverride_BloomThreshold;
	Volume.Settings.bOverride_BloomSizeScale = Setting->bOverride_BloomSizeScale;

	//exposure
	Volume.Settings.AutoExposureMethod = Setting->AutoExposureMethod;
	Volume.Settings.AutoExposureBias = Setting->AutoExposureBias;
	Volume.Settings.AutoExposureApplyPhysicalCameraExposure = Setting->AutoExposureApplyPhysicalCameraExposure;
	Volume.Settings.AutoExposureBiasCurve = Setting->AutoExposureBiasCurve;
	Volume.Settings.AutoExposureMeterMask = Setting->AutoExposureMeterMask;
	Volume.Settings.AutoExposureLowPercent = Setting->AutoExposureLowPercent;
	Volume.Settings.AutoExposureHighPercent = Setting->AutoExposureHighPercent;
	Volume.Settings.AutoExposureMinBrightness = Setting->AutoExposureMinBrightness;
	Volume.Settings.AutoExposureMaxBrightness = Setting->AutoExposureMaxBrightness;
	Volume.Settings.AutoExposureSpeedUp = Setting->AutoExposureSpeedUp;
	Volume.Settings.AutoExposureSpeedDown = Setting->AutoExposureSpeedDown;
	Volume.Settings.HistogramLogMin = Setting->HistogramLogMin;
	Volume.Settings.HistogramLogMax = Setting->HistogramLogMax;

	Volume.Settings.bOverride_AutoExposureMethod = Setting->bOverride_AutoExposureMethod;
	Volume.Settings.bOverride_AutoExposureBias = Setting->bOverride_AutoExposureBias;
	Volume.Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = Setting->bOverride_AutoExposureApplyPhysicalCameraExposure;
	Volume.Settings.bOverride_AutoExposureBiasCurve = Setting->bOverride_AutoExposureBiasCurve;
	Volume.Settings.bOverride_AutoExposureMeterMask = Setting->bOverride_AutoExposureMeterMask;
	Volume.Settings.bOverride_AutoExposureLowPercent = Setting->bOverride_AutoExposureLowPercent;
	Volume.Settings.bOverride_AutoExposureHighPercent = Setting->bOverride_AutoExposureHighPercent;
	Volume.Settings.bOverride_AutoExposureMinBrightness = Setting->bOverride_AutoExposureMinBrightness;
	Volume.Settings.bOverride_AutoExposureMaxBrightness = Setting->bOverride_AutoExposureMaxBrightness;
	Volume.Settings.bOverride_AutoExposureSpeedUp = Setting->bOverride_AutoExposureSpeedUp;
	Volume.Settings.bOverride_AutoExposureSpeedDown = Setting->bOverride_AutoExposureSpeedDown;
	Volume.Settings.bOverride_HistogramLogMin = Setting->bOverride_HistogramLogMin;
	Volume.Settings.bOverride_HistogramLogMax = Setting->bOverride_HistogramLogMax;


	//film
	Volume.Settings.FilmSlope = Setting->FilmSlope;
	Volume.Settings.FilmToe = Setting->FilmToe;
	Volume.Settings.FilmShoulder = Setting->FilmShoulder;
	Volume.Settings.FilmBlackClip = Setting->FilmBlackClip;
	Volume.Settings.FilmWhiteClip = Setting->FilmWhiteClip;

	Volume.Settings.bOverride_FilmSlope = Setting->bOverride_FilmSlope;
	Volume.Settings.bOverride_FilmToe = Setting->bOverride_FilmToe;
	Volume.Settings.bOverride_FilmShoulder = Setting->bOverride_FilmShoulder;
	Volume.Settings.bOverride_FilmBlackClip = Setting->bOverride_FilmBlackClip;
	Volume.Settings.bOverride_FilmWhiteClip = Setting->bOverride_FilmWhiteClip;



	//Rendering Features
	Volume.Settings.AmbientOcclusionIntensity = Setting->AmbientOcclusionIntensity;
	Volume.Settings.AmbientOcclusionRadius = Setting->AmbientOcclusionRadius;
	Volume.Settings.AmbientOcclusionStaticFraction = Setting->AmbientOcclusionStaticFraction;
	Volume.Settings.AmbientOcclusionRadiusInWS = Setting->AmbientOcclusionRadiusInWS;
	Volume.Settings.AmbientOcclusionFadeDistance = Setting->AmbientOcclusionFadeDistance;
	Volume.Settings.AmbientOcclusionFadeRadius = Setting->AmbientOcclusionFadeRadius;
	Volume.Settings.AmbientOcclusionPower = Setting->AmbientOcclusionPower;
	Volume.Settings.AmbientOcclusionBias = Setting->AmbientOcclusionBias;
	Volume.Settings.AmbientOcclusionQuality = Setting->AmbientOcclusionQuality;
	Volume.Settings.AmbientOcclusionMipBlend = Setting->AmbientOcclusionMipBlend;
	Volume.Settings.AmbientOcclusionMipScale = Setting->AmbientOcclusionMipScale;
	Volume.Settings.AmbientOcclusionMipThreshold = Setting->AmbientOcclusionMipThreshold;
	Volume.Settings.AmbientOcclusionOffsetScale = Setting->AmbientOcclusionOffsetScale;
	Volume.Settings.AmbientOcclusionEdgeHighlight = Setting->AmbientOcclusionEdgeHighlight;

	Volume.Settings.bOverride_AmbientOcclusionIntensity = Setting->bOverride_AmbientOcclusionIntensity;
	Volume.Settings.bOverride_AmbientOcclusionRadius = Setting->bOverride_AmbientOcclusionRadius;
	Volume.Settings.bOverride_AmbientOcclusionStaticFraction = Setting->bOverride_AmbientOcclusionStaticFraction;
	Volume.Settings.bOverride_AmbientOcclusionRadiusInWS = Setting->bOverride_AmbientOcclusionRadiusInWS;
	Volume.Settings.bOverride_AmbientOcclusionFadeDistance = Setting->bOverride_AmbientOcclusionFadeDistance;
	Volume.Settings.bOverride_AmbientOcclusionFadeRadius = Setting->bOverride_AmbientOcclusionFadeRadius;
	Volume.Settings.bOverride_AmbientOcclusionPower = Setting->bOverride_AmbientOcclusionPower;
	Volume.Settings.bOverride_AmbientOcclusionBias = Setting->bOverride_AmbientOcclusionBias;
	Volume.Settings.bOverride_AmbientOcclusionQuality = Setting->bOverride_AmbientOcclusionQuality;
	Volume.Settings.bOverride_AmbientOcclusionMipBlend = Setting->bOverride_AmbientOcclusionMipBlend;

	Volume.Settings.bOverride_AmbientOcclusionMipScale = Setting->bOverride_AmbientOcclusionMipScale;
	Volume.Settings.bOverride_AmbientOcclusionMipThreshold = Setting->bOverride_AmbientOcclusionMipThreshold;
	Volume.Settings.bOverride_AmbientOcclusionOffsetScale = Setting->bOverride_AmbientOcclusionOffsetScale;
	Volume.Settings.bOverride_AmbientOcclusionEdgeHighlight = Setting->bOverride_AmbientOcclusionEdgeHighlight;
}


void SPostProcessManager::GetPostProcessParams(APostProcessVolume& Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdatePostProcessVolumeParams :  %s"), *Volume.GetName());

	//bloom
	Setting->BloomMethod = Volume.Settings.BloomMethod;
	Setting->BloomIntensity = Volume.Settings.BloomIntensity;
	Setting->BloomThreshold = Volume.Settings.BloomThreshold;
	Setting->BloomSizeScale = Volume.Settings.BloomSizeScale;

	Setting->bOverride_BloomMethod = Volume.Settings.bOverride_BloomMethod ;
	Setting->bOverride_BloomIntensity = Volume.Settings.bOverride_BloomIntensity  ;
	Setting->bOverride_BloomThreshold = Volume.Settings.bOverride_BloomThreshold  ;
	Setting->bOverride_BloomSizeScale = Volume.Settings.bOverride_BloomSizeScale  ;

	//exposure
	Setting->AutoExposureMethod = Volume.Settings.AutoExposureMethod;
	Setting->AutoExposureBias = Volume.Settings.AutoExposureBias;
	Setting->AutoExposureApplyPhysicalCameraExposure = Volume.Settings.AutoExposureApplyPhysicalCameraExposure ;
	Setting->AutoExposureBiasCurve = Volume.Settings.AutoExposureBiasCurve;
	Setting->AutoExposureMeterMask = Volume.Settings.AutoExposureMeterMask;
	Setting->AutoExposureLowPercent = Volume.Settings.AutoExposureLowPercent;
	Setting->AutoExposureHighPercent = Volume.Settings.AutoExposureHighPercent ;
	Setting->AutoExposureMinBrightness = Volume.Settings.AutoExposureMinBrightness  ;
	Setting->AutoExposureMaxBrightness = Volume.Settings.AutoExposureMaxBrightness  ;
	Setting->AutoExposureSpeedUp = Volume.Settings.AutoExposureSpeedUp  ;
	Setting->AutoExposureSpeedDown = Volume.Settings.AutoExposureSpeedDown  ;
	Setting->HistogramLogMin = Volume.Settings.HistogramLogMin ;
	Setting->HistogramLogMax = Volume.Settings.HistogramLogMax  ;

	Setting->bOverride_AutoExposureMethod = Volume.Settings.bOverride_AutoExposureMethod  ;
	Setting->bOverride_AutoExposureBias = Volume.Settings.bOverride_AutoExposureBias  ;
	Setting->bOverride_AutoExposureApplyPhysicalCameraExposure = Volume.Settings.bOverride_AutoExposureApplyPhysicalCameraExposure  ;
	Setting->bOverride_AutoExposureBiasCurve = Volume.Settings.bOverride_AutoExposureBiasCurve  ;
	Setting->bOverride_AutoExposureMeterMask = Volume.Settings.bOverride_AutoExposureMeterMask  ;
	Setting->bOverride_AutoExposureLowPercent = Volume.Settings.bOverride_AutoExposureLowPercent  ;
	Setting->bOverride_AutoExposureHighPercent = Volume.Settings.bOverride_AutoExposureHighPercent  ;
	Setting->bOverride_AutoExposureMinBrightness = Volume.Settings.bOverride_AutoExposureMinBrightness  ;
	Setting->bOverride_AutoExposureMaxBrightness = Volume.Settings.bOverride_AutoExposureMaxBrightness ;
	Setting->bOverride_AutoExposureSpeedUp = Volume.Settings.bOverride_AutoExposureSpeedUp  ;
	Setting->bOverride_AutoExposureSpeedDown = Volume.Settings.bOverride_AutoExposureSpeedDown  ;
	Setting->bOverride_HistogramLogMin = Volume.Settings.bOverride_HistogramLogMin  ;
	Setting->bOverride_HistogramLogMax = Volume.Settings.bOverride_HistogramLogMax  ;

	//film
	Setting->FilmSlope = Volume.Settings.FilmSlope  ;
	Setting->FilmToe = Volume.Settings.FilmToe  ;
	Setting->FilmShoulder = Volume.Settings.FilmShoulder  ;
	Setting->FilmBlackClip = Volume.Settings.FilmBlackClip  ;
	Setting->FilmWhiteClip = Volume.Settings.FilmWhiteClip  ;

	Setting->bOverride_FilmSlope = Volume.Settings.bOverride_FilmSlope  ;
	Setting->bOverride_FilmToe = Volume.Settings.bOverride_FilmToe  ;
	Setting->bOverride_FilmShoulder = Volume.Settings.bOverride_FilmShoulder  ;
	Setting->bOverride_FilmBlackClip = Volume.Settings.bOverride_FilmBlackClip  ;
	Setting->bOverride_FilmWhiteClip = Volume.Settings.bOverride_FilmWhiteClip  ;

	//Rendering Features
	Setting->AmbientOcclusionIntensity = Volume.Settings.AmbientOcclusionIntensity  ;
	Setting->AmbientOcclusionRadius = Volume.Settings.AmbientOcclusionRadius  ;
	Setting->AmbientOcclusionStaticFraction = Volume.Settings.AmbientOcclusionStaticFraction  ;
	Setting->AmbientOcclusionRadiusInWS = Volume.Settings.AmbientOcclusionRadiusInWS  ;
	Setting->AmbientOcclusionFadeDistance = Volume.Settings.AmbientOcclusionFadeDistance  ;
	Setting->AmbientOcclusionFadeRadius = Volume.Settings.AmbientOcclusionFadeRadius  ;
	Setting->AmbientOcclusionPower = Volume.Settings.AmbientOcclusionPower ;
	Setting->AmbientOcclusionBias = Volume.Settings.AmbientOcclusionBias ;
	Setting->AmbientOcclusionQuality = Volume.Settings.AmbientOcclusionQuality  ;
	Setting->AmbientOcclusionMipBlend = Volume.Settings.AmbientOcclusionMipBlend ;
	Setting->AmbientOcclusionMipScale = Volume.Settings.AmbientOcclusionMipScale  ;
	Setting->AmbientOcclusionMipThreshold = Volume.Settings.AmbientOcclusionMipThreshold  ;
	Setting->AmbientOcclusionOffsetScale = Volume.Settings.AmbientOcclusionOffsetScale  ;
	Setting->AmbientOcclusionEdgeHighlight = Volume.Settings.AmbientOcclusionEdgeHighlight  ;

	Setting->bOverride_AmbientOcclusionIntensity = Volume.Settings.bOverride_AmbientOcclusionIntensity  ;
	Setting->bOverride_AmbientOcclusionRadius = Volume.Settings.bOverride_AmbientOcclusionRadius  ;
	Setting->bOverride_AmbientOcclusionStaticFraction = Volume.Settings.bOverride_AmbientOcclusionStaticFraction ;
	Setting->bOverride_AmbientOcclusionRadiusInWS = Volume.Settings.bOverride_AmbientOcclusionRadiusInWS  ;
	Setting->bOverride_AmbientOcclusionFadeDistance = Volume.Settings.bOverride_AmbientOcclusionFadeDistance  ;
	Setting->bOverride_AmbientOcclusionFadeRadius = Volume.Settings.bOverride_AmbientOcclusionFadeRadius  ;
	Setting->bOverride_AmbientOcclusionPower = Volume.Settings.bOverride_AmbientOcclusionPower  ;
	Setting->bOverride_AmbientOcclusionBias = Volume.Settings.bOverride_AmbientOcclusionBias  ;
	Setting->bOverride_AmbientOcclusionQuality = Volume.Settings.bOverride_AmbientOcclusionQuality  ;
	Setting->bOverride_AmbientOcclusionMipBlend = Volume.Settings.bOverride_AmbientOcclusionMipBlend  ;
	Setting->bOverride_AmbientOcclusionMipScale = Volume.Settings.bOverride_AmbientOcclusionMipScale ;
	Setting->bOverride_AmbientOcclusionMipThreshold = Volume.Settings.bOverride_AmbientOcclusionMipThreshold ;
	Setting->bOverride_AmbientOcclusionOffsetScale = Volume.Settings.bOverride_AmbientOcclusionOffsetScale  ;
	Setting->bOverride_AmbientOcclusionEdgeHighlight = Volume.Settings.bOverride_AmbientOcclusionEdgeHighlight  ;
}
