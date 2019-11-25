// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/SceneComponent.h"
#include "PickupableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface that represents pickupable object.
 */
class VRPACK_API IPickupableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Puckupable Object")
	void OnAttach(USceneComponent* Parent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Puckupable Object")
	void OnDetach();

};
