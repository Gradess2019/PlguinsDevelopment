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
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		float SliceSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		float AllowableAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		float SliceAngleTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		bool CastShadow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		FName CollisionPreset;

	UPROPERTY(EditDefaultsOnly, Category = "Paint component")
		UStaticMesh* StaticMesh;

	FPictureSettings()
	{
		SliceSize = 10.f;
		AllowableAngle = 40.f;
		SliceAngleTolerance = 0.1f;

		CastShadow = false;

		CollisionPreset = FName("BlockAll");

		StaticMesh = nullptr;
	}
};

struct FSplineMeshInitializer;

USTRUCT(BlueprintType)
struct FSplineMeshInitializer
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		FVector StartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		FVector StartTangent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		FVector EndPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Picture settings")
		FVector EndTangent;

	FSplineMeshInitializer(FVector StartPos, FVector StartTangent, FVector EndPos, FVector EndTangent) :
		StartPos(StartPos), StartTangent(StartTangent), EndPos(EndPos), EndTangent(EndTangent) {}

	FSplineMeshInitializer() : StartPos(FVector::ZeroVector), StartTangent(FVector::ZeroVector), EndPos(FVector::ZeroVector), EndTangent(FVector::ZeroVector) {}

};

USTRUCT(BlueprintType)
struct FStaticMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Data")
	TWeakObjectPtr<UStaticMeshComponent> Component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Data")
	bool SimulatePhysics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Data")
	FName CollisionProfileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh Data")
	FCollisionResponseContainer ResponseData;

	FStaticMeshData() : SimulatePhysics(false), CollisionProfileName(NAME_None) {}

	FStaticMeshData(UStaticMeshComponent* Component)
	{
		SaveData(Component);
	}

	void SaveData(UStaticMeshComponent* Component)
	{
		this->Component = Component;
		SimulatePhysics = Component->IsSimulatingPhysics();
		CollisionProfileName = Component->GetCollisionProfileName();
		ResponseData = Component->GetCollisionResponseToChannels();
	}

	void ApplySavedData() const
	{
		Component->SetSimulatePhysics(SimulatePhysics);
		Component->SetCollisionProfileName(CollisionProfileName);
		Component->SetCollisionResponseToChannels(ResponseData);
	}
};