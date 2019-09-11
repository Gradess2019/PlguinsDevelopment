#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"

UCLASS()
class VRTELEPORT_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AVRCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
