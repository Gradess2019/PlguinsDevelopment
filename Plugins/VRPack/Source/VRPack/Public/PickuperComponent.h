// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PickuperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRPACK_API UPickuperComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UPickuperComponent();

	UFUNCTION(BlueprintCallable, Category = "Pickuper Component")
	void Pickup();
	
	UFUNCTION(BlueprintCallable, Category = "Pickuper Component")
	void Throw();

private:

	TWeakObjectPtr<AActor*> PickupedActor;
	
};
