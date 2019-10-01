// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "Picture.generated.h"

UCLASS()
class VRPACK_API APicture : public AActor
{
	GENERATED_BODY()
	
public:

	APicture(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void InitializePicture(float PictureSliceSize, UStaticMesh* PictureMesh);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void Follow(FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void CreateNewMesh(FVector PointLocation);
	
private:
	
	float SliceSize;

	FVector LastDrewLocation;

	UStaticMesh* Mesh;
	
	TWeakObjectPtr<USplineMeshComponent> CurrentSlice;
	TWeakObjectPtr<USplineMeshComponent> LastDrawnSlice;
	
	void DrawNext(FVector PointLocation);

};
