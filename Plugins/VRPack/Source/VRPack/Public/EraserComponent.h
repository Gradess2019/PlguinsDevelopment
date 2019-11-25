// Copyright 2017-2019 MARTA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "EraserComponent.generated.h"

/**
 * Use such as eraser for picture
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VRPACK_API UEraserComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eraser component")
	TSubclassOf<AActor> ClassToErase;
	
private:

	UFUNCTION()
	void OnEraserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
