#pragma once
#include "CoreMinimal.h"
#include "TeleportStructs.generated.h"

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