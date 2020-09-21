// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Scene.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelPostProcessSettings.generated.h"



/**
 * 
 */
UCLASS()
class SCENEMANAGER_API ULevelPostProcessSettings : public UObject
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Category = "PostProcess")
	//	float param1;


	//UPROPERTY(EditAnywhere, Category = "PostProcess")
	//	FVector param2;

	//UPROPERTY(EditAnywhere, Category = "PostProcess")
	FPostProcessSettings postprocess;



	//---------------------------------------------Lengs -------------------------------------------------------

	//Bloom
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_BloomMethod : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_BloomIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_BloomThreshold : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_BloomSizeScale : 1;


	/** Bloom algorithm */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lens|Bloom", meta = (editcondition = "bOverride_BloomMethod", DisplayName = "Method"))
		TEnumAsByte<enum EBloomMethod> BloomMethod;

	/** Multiplier for all bloom contributions >=0: off, 1(default), >1 brighter */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Bloom", meta = (ClampMin = "0.0", UIMax = "8.0", editcondition = "bOverride_BloomIntensity", DisplayName = "Intensity"))
		float BloomIntensity;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Bloom", meta = (ClampMin = "-1.0", UIMax = "8.0", editcondition = "bOverride_BloomThreshold", DisplayName = "Threshold"))
		float BloomThreshold;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Bloom|Advanced", meta = (ClampMin = "0.0", UIMax = "64.0", editcondition = "bOverride_BloomSizeScale", DisplayName = "Size scale"))
		float BloomSizeScale;





	//Exposure
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureMethod : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureLowPercent : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureHighPercent : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureMinBrightness : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureMaxBrightness : 1;

	UPROPERTY()
		uint8 bOverride_AutoExposureCalibrationConstant_DEPRECATED : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureSpeedUp : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureSpeedDown : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureBias : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureBiasCurve : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureMeterMask : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AutoExposureApplyPhysicalCameraExposure : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_HistogramLogMin : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_HistogramLogMax : 1;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lens|Exposure", meta = (editcondition = "bOverride_AutoExposureMethod", DisplayName = "Metering Mode"))
		TEnumAsByte<enum EAutoExposureMethod> AutoExposureMethod;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure", meta = (UIMin = "-15.0", UIMax = "15.0", editcondition = "bOverride_AutoExposureBias", DisplayName = "Exposure Compensation "))
		float AutoExposureBias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lens|Exposure", meta = (editcondition = "bOverride_AutoExposureApplyPhysicalCameraExposure", DisplayName = "Apply Physical Camera Exposure", tooltip = "Only affects Manual exposure mode."))
		uint32 AutoExposureApplyPhysicalCameraExposure : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lens|Exposure", meta = (editcondition = "bOverride_AutoExposureBiasCurve", DisplayName = "Exposure Compensation Curve"))
		class UCurveFloat* AutoExposureBiasCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lens|Exposure", meta = (editcondition = "bOverride_AutoExposureMeterMask", DisplayName = "Exposure Metering Mask"))
		class UTexture* AutoExposureMeterMask = nullptr;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure|Advanced", meta = (ClampMin = "0.0", ClampMax = "100.0", editcondition = "bOverride_AutoExposureLowPercent", DisplayName = "Low Percent"))
		float AutoExposureLowPercent;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure|Advanced", meta = (ClampMin = "0.0", ClampMax = "100.0", editcondition = "bOverride_AutoExposureHighPercent", DisplayName = "High Percent"))
		float AutoExposureHighPercent;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure", meta = (ClampMin = "-10.0", UIMax = "20.0", editcondition = "bOverride_AutoExposureMinBrightness", DisplayName = "Min Brightness"))
		float AutoExposureMinBrightness;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure", meta = (ClampMin = "-10.0", UIMax = "20.0", editcondition = "bOverride_AutoExposureMaxBrightness", DisplayName = "Max Brightness"))
		float AutoExposureMaxBrightness;


	/** >0 */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure", meta = (ClampMin = "0.02", UIMax = "20.0", editcondition = "bOverride_AutoExposureSpeedUp", DisplayName = "Speed Up", tooltip = "In F-stops per second, should be >0"))
		float AutoExposureSpeedUp;

	/** >0 */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure", meta = (ClampMin = "0.02", UIMax = "20.0", editcondition = "bOverride_AutoExposureSpeedDown", DisplayName = "Speed Down", tooltip = "In F-stops per second, should be >0"))
		float AutoExposureSpeedDown;

	/** Histogram Min value. Expressed in Log2(Luminance) or in EV100 when using ExtendDefaultLuminanceRange (see project settings) */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure|Advanced", meta = (UIMin = "-16", UIMax = "0.0", editcondition = "bOverride_HistogramLogMin"))
		float HistogramLogMin;

	/** Histogram Max value. Expressed in Log2(Luminance) or in EV100 when using ExtendDefaultLuminanceRange (see project settings) */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Lens|Exposure|Advanced", meta = (UIMin = "0.0", UIMax = "16.0", editcondition = "bOverride_HistogramLogMax"))
		float HistogramLogMax;



	//---------------------------------------------Film -------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_FilmSlope : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_FilmToe : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_FilmShoulder : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_FilmBlackClip : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_FilmWhiteClip : 1;


	UPROPERTY(interp, BlueprintReadWrite, Category = "Film", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bOverride_FilmSlope", DisplayName = "Slope"))
		float FilmSlope;
	UPROPERTY(interp, BlueprintReadWrite, Category = "Film", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bOverride_FilmToe", DisplayName = "Toe"))
		float FilmToe;
	UPROPERTY(interp, BlueprintReadWrite, Category = "Film", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bOverride_FilmShoulder", DisplayName = "Shoulder"))
		float FilmShoulder;
	UPROPERTY(interp, BlueprintReadWrite, Category = "Film", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bOverride_FilmBlackClip", DisplayName = "Black clip"))
		float FilmBlackClip;
	UPROPERTY(interp, BlueprintReadWrite, Category = "Film", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bOverride_FilmWhiteClip", DisplayName = "White clip"))
		float FilmWhiteClip;



	//---------------------------------------------Rendering Features -------------------------------------------------------


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionIntensity : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionStaticFraction : 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionRadius : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionFadeDistance : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionFadeRadius : 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionRadiusInWS : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionPower : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionBias : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionQuality : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionMipBlend : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionMipScale : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionMipThreshold : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionOffsetScale : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		uint8 bOverride_AmbientOcclusionEdgeHighlight : 1;


	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion", meta = (ClampMin = "0.0", ClampMax = "1.0", editcondition = "bOverride_AmbientOcclusionIntensity", DisplayName = "Intensity"))
		float AmbientOcclusionIntensity;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion", meta = (ClampMin = "0.1", UIMax = "500.0", editcondition = "bOverride_AmbientOcclusionRadius", DisplayName = "Radius"))
		float AmbientOcclusionRadius;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", ClampMax = "1.0", editcondition = "bOverride_AmbientOcclusionStaticFraction", DisplayName = "Static Fraction"))
		float AmbientOcclusionStaticFraction;



	/** true: AO radius is in world space units, false: AO radius is locked the view space in 400 units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (editcondition = "bOverride_AmbientOcclusionRadiusInWS", DisplayName = "Radius in WorldSpace"))
		uint32 AmbientOcclusionRadiusInWS : 1;

	/** >0, in unreal units, at what distance the AO effect disppears in the distance (avoding artifacts and AO effects on huge object) */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion", AdvancedDisplay, meta = (ClampMin = "0.0", UIMax = "20000.0", editcondition = "bOverride_AmbientOcclusionFadeDistance", DisplayName = "Fade Out Distance"))
		float AmbientOcclusionFadeDistance;

	/** >0, in unreal units, how many units before AmbientOcclusionFadeOutDistance it starts fading out */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "20000.0", editcondition = "bOverride_AmbientOcclusionFadeRadius", DisplayName = "Fade Out Radius"))
		float AmbientOcclusionFadeRadius;

	/** >0, in unreal units, how wide the ambient occlusion effect should affect the geometry (in depth), will be removed - only used for non normal method which is not exposed */
	UPROPERTY()
		float AmbientOcclusionDistance_DEPRECATED;

	/** >0, in unreal units, bigger values means even distant surfaces affect the ambient occlusion */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.1", UIMax = "8.0", editcondition = "bOverride_AmbientOcclusionPower", DisplayName = "Power"))
		float AmbientOcclusionPower;

	/** >0, in unreal units, default (3.0) works well for flat surfaces but can reduce details */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "10.0", editcondition = "bOverride_AmbientOcclusionBias", DisplayName = "Bias"))
		float AmbientOcclusionBias;

	/** 0=lowest quality..100=maximum quality, only a few quality levels are implemented, no soft transition */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "100.0", editcondition = "bOverride_AmbientOcclusionQuality", DisplayName = "Quality"))
		float AmbientOcclusionQuality;

	/** Affects the blend over the multiple mips (lower resolution versions) , 0:fully use full resolution, 1::fully use low resolution, around 0.6 seems to be a good value */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.1", UIMax = "1.0", editcondition = "bOverride_AmbientOcclusionMipBlend", DisplayName = "Mip Blend"))
		float AmbientOcclusionMipBlend;

	/** Affects the radius AO radius scale over the multiple mips (lower resolution versions) */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.5", UIMax = "4.0", editcondition = "bOverride_AmbientOcclusionMipScale", DisplayName = "Mip Scale"))
		float AmbientOcclusionMipScale;

	/** to tweak the bilateral upsampling when using multiple mips (lower resolution versions) */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "0.1", editcondition = "bOverride_AmbientOcclusionMipThreshold", DisplayName = "Mip Threshold"))
		float AmbientOcclusionMipThreshold;

	/** The distance of the first sample in [0, 1]*/
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "1.0", editcondition = "bOverride_AmbientOcclusionOffsetScale", DisplayName = "Offset Scale"))
		float AmbientOcclusionOffsetScale;

	/** Multiplier for edge highlighting in [0, 1]  */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Rendering Features|Ambient Occlusion|Advanced", meta = (ClampMin = "0.0", UIMax = "1.0", editcondition = "bOverride_AmbientOcclusionEdgeHighlight", DisplayName = "Edge Highlight"))
		float AmbientOcclusionEdgeHighlight;
};
