// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPickuperComponent::UPickuperComponent()
{
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxExtent = FVector(10.f);
}

void UPickuperComponent::Pickup()
{
	if (OverlappingComponents.Num() == 0) { return; }

	OverlappedObject = OverlappingComponents[0].OverlapInfo.Component;

	PickupedObject = OverlappedObject;
	PickupedObject->SetSimulatePhysics(false);
	PickupedObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PickupedObject->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
	PickupedObject->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
}

void UPickuperComponent::Throw()
{
	if (!PickupedObject.IsValid()) return;

	PickupedObject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	PickupedObject->SetSimulatePhysics(true);
	PickupedObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	PickupedObject->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Block);
	PickupedObject = nullptr;
}

