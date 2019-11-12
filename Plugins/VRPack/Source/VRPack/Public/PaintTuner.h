// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PaintComponent.h"
#include "PaintTuner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPaintTuner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRPACK_API IPaintTuner
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Paint Component")
	void Init(UPaintComponent* Component);
	
};
