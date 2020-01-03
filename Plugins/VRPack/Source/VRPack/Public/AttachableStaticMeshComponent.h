// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PickupableObject.h"
#include "Components/StaticMeshComponent.h"
#include "AttachableStaticMeshComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttached, USceneComponent*, Parent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetached);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VRPACK_API UAttachableStaticMeshComponent : public UStaticMeshComponent, public IPickupableObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Attachable Component")
	FOnAttached OnAttached;

	UPROPERTY(BlueprintAssignable, Category = "Attachable Component")
	FOnDetached OnDetached;
	
	void OnAttach_Implementation(USceneComponent* Parent) override;
	
	void OnDetach_Implementation() override;
	
};
