// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeightChangerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VRPACK_API UHeightChangerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHeightChangerComponent();
	
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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Height changer component")
	float GetHeight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Height changer component")
	float GetMaxHeight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Height changer component")
	float GetMinHeight() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Changer settings")
	FName TargetComponentName;
	
	UPROPERTY(BlueprintReadOnly, Category = "Changer settings")
	TWeakObjectPtr<USceneComponent> TargetComponent;

private:

	float CurrentHeight;
	float HeightCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float Increment;
	
	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float Decrement;

	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float MaxHeight;
	
	UPROPERTY(EditDefaultsOnly, Category = "Changer settings")
	float MinHeight;


	UFUNCTION()
	void BeginPlay() override;
	
};
