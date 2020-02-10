// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Invalidator.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInvalidator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRPACK_API IInvalidator
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Invalidator")
	void Invalidate();
	
};
