// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "VRPackStructs.h"
#include "PickupableObject.h"
#include "Picture.generated.h"

UCLASS()
class VRPACK_API APicture : public AActor, public IPickupableObject
{
	GENERATED_BODY()

public:

	APicture(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "Picture")
	void InitializePicture(FPictureSettings PictureSettings);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void UpdatePictureSettings(FPictureSettings PictureSettings);
	
	UFUNCTION(BlueprintCallable, Category = "Picture")
	void EnableCollision();

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void FinishFollowing();

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void Follow(FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void CreateNewMesh(FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void SetVisible(bool bVisible);

	void OnAttach_Implementation(USceneComponent* Parent) override;
	void OnDetach_Implementation() override;
	
private:

	FPictureSettings PictureSettings;

	TWeakObjectPtr<USplineMeshComponent> CurrentSlice;

	TWeakObjectPtr<UMaterialInstanceDynamic> Material;

	TWeakObjectPtr<USplineComponent> Spline;

	bool IsAllowableSize() const;
	bool IsAllowableAngle() const;
	int32 GetPreviousPointIndex() const;
	float GetAngle() const;
	int32 GetCurrentPointIndex() const;

	FTransform GetParentTransform() const;
	void SetMaterial();
	void CreateMaterial();
	void SetStartAndEnd(const FSplineMeshInitializer& Initializer) const;
	void CreateSplineMeshComponent();
};
