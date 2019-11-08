// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "VRPackStructs.h"
#include "Picture.generated.h"

UCLASS()
class VRPACK_API APicture : public AActor
{
	GENERATED_BODY()

public:

	APicture(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void InitializePicture(FPictureSettings PictureSettings);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void FinishFollowing();

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void Follow(FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void CreateNewMesh(FVector PointLocation);

private:

	FPictureSettings PictureSettings;

	TWeakObjectPtr<USplineMeshComponent> CurrentSlice;
	TWeakObjectPtr<USplineMeshComponent> LastDrawnSlice;

	bool IsAllowableSize() const;
	bool IsAllowableAngle() const;
	float GetAngle() const;
	FVector GetNormalizedDirection(const USplineMeshComponent* SplineMesh) const;
	void CalculateSplineTangentAndPositions(FVector PointLocation) const;

	FTransform GetParentTransform() const;
	void SetStartAndEnd(const FSplineMeshInitializer& Initializer) const;
	void CreateSplineMeshComponent(FVector RelativeLocation);

	
};
