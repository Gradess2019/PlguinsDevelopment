#pragma once
#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "VRPackStructs.generated.h"

USTRUCT(BlueprintType)
struct FFadeSettings
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	float FromAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	float ToAlpha;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	bool bShouldFadeAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	bool bHoldWhenFinished;

	FFadeSettings()
	{
		FromAlpha = 0.f;
		ToAlpha = 0.f;
		Duration = 0.f;

		Color = FColor::Black;

		bShouldFadeAudio = false;
		bHoldWhenFinished = true;
	}
};

USTRUCT(BlueprintType)
struct FTimelineSettings
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	bool Loop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	bool IgnoreTimeDilation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade Settings")
	TEnumAsByte<ETimelineLengthMode> Mode;

	FTimelineSettings()
	{
		Loop = false;
		IgnoreTimeDilation = false;

		Length = 5.f;
		PlayRate = 1.f;

		Mode = ETimelineLengthMode::TL_TimelineLength;
	}
};

USTRUCT(BlueprintType)
struct FPictureSettings
{
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
	float SliceSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
	float AllowableAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
	float SliceAngleTolerance;


	
	UPROPERTY(EditDefaultsOnly, Category = "Paint component")
	UStaticMesh* StaticMesh;

	
};