// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeightChangerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VRPACK_API UHeightChangerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	void Raise();
	
	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	void Lower();

	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	void SetCustomHeight(float NewHeight);

	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	void SetIncrement(float NewIncrement);

	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	void SetDecrement(float NewDecrement);

	UFUNCTION(BlueprintCallable, Category = "Height changer component")
	float GetHeight();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Changer settings")
	FName TargetComponentName;
	
	UPROPERTY(BlueprintReadOnly, Category = "Changer settings")
	TWeakObjectPtr<USceneComponent> TargetComponent;

private:

	float CurrentHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float Increment;
	
	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float Decrement;

	UFUNCTION()
	void BeginPlay() override;
	
};
