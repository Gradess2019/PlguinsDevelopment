#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "VRCharacter.generated.h"

UCLASS()
class PLUGINSDEVELOPMENT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AVRCharacter(const FObjectInitializer& OBJECT_INITIALIZER);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character settings")
	TWeakObjectPtr<UMotionControllerComponent> teleportController;

	UPROPERTY(EditDefaultsOnly, Category = "Character settings")
	FName teleportButtonActionMapping;

private:

	UFUNCTION()
	void OnTeleportButtonPressed();
	
	UFUNCTION()
	void OnTeleportButtonReleased();
};
