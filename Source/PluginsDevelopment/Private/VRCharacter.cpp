#include "VRCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine.h"

AVRCharacter::AVRCharacter(const FObjectInitializer& OBJECT_INITIALIZER) : Super(OBJECT_INITIALIZER)
{
	teleportController = OBJECT_INITIALIZER.CreateDefaultSubobject<UMotionControllerComponent>(this, "Motion controller");
	teleportController->SetupAttachment(GetMesh());
	teleportController->bDisplayDeviceModel = true;
	teleportController->RegisterComponent();
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(teleportButtonActionMapping, EInputEvent::IE_Pressed, this, &AVRCharacter::OnTeleportButtonPressed);
	PlayerInputComponent->BindAction(teleportButtonActionMapping, EInputEvent::IE_Released, this, &AVRCharacter::OnTeleportButtonReleased);
}

void AVRCharacter::OnTeleportButtonPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Teleport pressed"));
}

void AVRCharacter::OnTeleportButtonReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Teleport released"));
}

