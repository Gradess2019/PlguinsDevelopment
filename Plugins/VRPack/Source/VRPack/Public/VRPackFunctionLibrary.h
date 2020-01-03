// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRPackFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VRPACK_API UVRPackFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	template <typename T, typename S>
	static FORCEINLINE void AppendArray(TArray<T*>& TargetArray, TArray<S*>& Source)
	{
		for (S* Object : Source)
		{
			TargetArray.Add(Object);
		}
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VRPackFunctionLibrary")
	static FRotator ClampRotator(FRotator Rotator);
	
};
