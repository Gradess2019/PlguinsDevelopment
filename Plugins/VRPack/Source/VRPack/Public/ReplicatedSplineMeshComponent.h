// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "VRPackStructs.h"
#include "ReplicatedSplineMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class VRPACK_API UReplicatedSplineMeshComponent : public USplineMeshComponent
{
	GENERATED_BODY()

public:

	UReplicatedSplineMeshComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Picture")
	void ApplyPictureSettings(const FPictureSettings& PictureSettings);
};
