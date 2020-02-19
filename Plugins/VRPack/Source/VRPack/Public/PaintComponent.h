// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "VRPackStructs.h"
#include "Picture.h"
#include "PaintComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinsihDrawing);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class VRPACK_API UPaintComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UPaintComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "Paint component")
	FOnFinsihDrawing OnFinsihDrawing;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Paint component")
	void StartDrawing();

	void virtual StartDrawing_Implementation();
	
	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void StopDrawing();

	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void FinishDrawing();

	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void SetLineWidth(float NewWidth);
	void UpdatePictureSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Paint component")
	void SetLineColor(const FLinearColor& NewColor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Paint component")
	APicture* GetCurrentPicture();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Paint component")
	bool IsDrawing();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Paint component")
	FPictureSettings PictureSettings;

	UPROPERTY(BlueprintReadOnly, Category = "Paint component")
	UTimelineComponent* DrawingTimeline;

	UPROPERTY(BlueprintReadWrite, Category = "Paint component")
	APicture* CurrentPicture;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Paint component")
	TSubclassOf<APicture> PictureClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Paint component")
	FTimelineSettings TimelineSettings;

	

	void InitializeComponent() override;
	void InitializeDrawingTimeline();

	UFUNCTION()
	void OnUpdatePaint();
};
