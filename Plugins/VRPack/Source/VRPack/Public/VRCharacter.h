#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionControllerComponent.h"
#include "TeleportComponent.h"
#include "Camera/CameraComponent.h"
#include "InputChord.h"
#include "VRCharacter.generated.h"

UCLASS()
class VRPACK_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AVRCharacter(const FObjectInitializer& OBJECT_INITIALIZER);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character settings")
	UTeleportComponent* TeleportComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character settings")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character settings")
	USceneComponent* VROrigin;

	UPROPERTY(EditDefaultsOnly, Category = "Character settings")
	TArray<FInputChord> TeleportationChords;
	
	UFUNCTION()
	void BeginPlay() override;
	
private:
	
	template<typename T>
	T* InitializeCustomComponent(const FObjectInitializer& OBJECT_INITIALIZER, const FName& COMPONENT_NAME, bool autoAttach = true);

	void SetVROriginOffset();
	
	UFUNCTION()
	void OnTeleportButtonPressed();
	
	UFUNCTION()
	void OnTeleportButtonReleased();

};