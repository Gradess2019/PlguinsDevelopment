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

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void Follow(FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void InitializePicture(float SliceSize, UStaticMesh* Mesh);

protected:
	
	void BeginPlay() override;
	
private:

	
	float SliceSize;

	FVector LastDrewLocation;

	TWeakObjectPtr<USplineMeshComponent> CurrentSlice;
	TWeakObjectPtr<USplineMeshComponent> LastDrawnSlice;
	
	void DrawNext(FVector PointLocation);

};
