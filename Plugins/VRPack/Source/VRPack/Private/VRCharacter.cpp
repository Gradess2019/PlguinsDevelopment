#include "VRCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AVRCharacter::AVRCharacter(const FObjectInitializer& OBJECT_INITIALIZER) : Super(OBJECT_INITIALIZER)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	VROrigin = InitializeCustomComponent<USceneComponent>(OBJECT_INITIALIZER, "VROrigin", false);
	VROrigin->SetupAttachment(GetMesh());
	
	TeleportComponent = InitializeCustomComponent<UTeleportComponent>(OBJECT_INITIALIZER, "TeleportComponent");
	CameraComponent = InitializeCustomComponent<UCameraComponent>(OBJECT_INITIALIZER, "CameraComponent");
	SetVROriginOffset();
}

template <typename T>
T* AVRCharacter::InitializeCustomComponent(const FObjectInitializer& OBJECT_INITIALIZER, const FName& COMPONENT_NAME, bool autoAttach)
{
	T* newComponent = OBJECT_INITIALIZER.CreateDefaultSubobject<T>(this, COMPONENT_NAME);

	if (autoAttach && IsValid(VROrigin))
	{
		newComponent->SetupAttachment(VROrigin);
	}
	
	return newComponent;
}

void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	for (FInputChord TeleportationChord : TeleportationChords)
	{
		PlayerInputComponent->BindKey(TeleportationChord, EInputEvent::IE_Pressed, this, &AVRCharacter::OnTeleportButtonPressed);
		PlayerInputComponent->BindKey(TeleportationChord, EInputEvent::IE_Released, this, &AVRCharacter::OnTeleportButtonReleased);
	}
}

void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
}

void AVRCharacter::SetVROriginOffset()
{
	const FVector ORIGIN_OFFSET = FVector(0.f, 0.f, 70.f);
	VROrigin->AddRelativeLocation(ORIGIN_OFFSET);
}

void AVRCharacter::OnTeleportButtonPressed()
{
	TeleportComponent->StartTeleportProjection();
}

void AVRCharacter::OnTeleportButtonReleased()
{
	TeleportComponent->Teleport();
}

