// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Invalidator.h"
#include "VRPackStructs.h"
#include "PickuperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VRPACK_API UPickuperComponent : public UBoxComponent, public IInvalidator
{
	GENERATED_BODY()

public:

	UPickuperComponent();

	UFUNCTION(BlueprintCallable, Category = "Pickuper Component")
	void Pickup();
	
	UFUNCTION(BlueprintCallable, Category = "Pickuper Component")
	void Throw();

	void Invalidate_Implementation() override;

private:

	TWeakObjectPtr<UObject> PickupedObject;
	TArray<FStaticMeshData> StaticMeshDatas;
};
