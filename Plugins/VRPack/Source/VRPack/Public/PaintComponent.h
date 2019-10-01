// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "VRPackStructs.h"
#include "PaintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRPACK_API UPaintComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UPaintComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void StartDrawing();

	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void StopDrawing();

private:

	UStaticMesh* SplineMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Paint component")
	FTimelineSettings TimelineSettings;
	
	TWeakObjectPtr<UTimelineComponent> DrawingTimeline;

	void InitializeComponent() override;
	void InitializeDrawingTimeline();
};
