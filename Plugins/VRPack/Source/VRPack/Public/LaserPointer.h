#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineBaseTypes.h"
#include "LaserPointer.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class VRPACK_API ULaserPointer : public USplineMeshComponent
{
	GENERATED_BODY()

public:

	ULaserPointer();

	UFUNCTION(BlueprintCallable, Category = "Laser pointer settings")
	void EnableLaser();

	UFUNCTION(BlueprintCallable, Category = "Laser pointer settings")
	void DisableLaser();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Laser pointer settings")
	TArray<TEnumAsByte<ECollisionChannel>> ObjectChannels;

	UPROPERTY(EditDefaultsOnly, Category = "Laser pointer settings")
	TArray<AActor*> ActorsToIgnore;
	
	FCollisionObjectQueryParams ObjectQueryParams;

	FCollisionQueryParams CollisionQueryParams;
	
	UPROPERTY(EditDefaultsOnly, Category = "Laser pointer settings")
	float Distance;

	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetEnabled(bool bIsEnabled);
};


