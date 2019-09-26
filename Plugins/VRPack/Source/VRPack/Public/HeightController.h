// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeightChangerComponent.h"
#include "HeightController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHeightController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRPACK_API IHeightController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Height changer component")
	void SetHeightChanger(UHeightChangerComponent* NewComponent);
};
