// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"

UPickuperComponent::UPickuperComponent()
{
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxExtent = FVector(10.f);
	OnComponentBeginOverlap.AddDynamic(this, &UPickuperComponent::OnBeginOverlapPickupableObject);
	OnComponentEndOverlap.AddDynamic(this, &UPickuperComponent::OnEndOverlapPickupableObject);
}

void UPickuperComponent::Pickup()
{
	if (!OverlappedObject.IsValid()) return;

	OverlappedObject->SetSimulatePhysics(false);
	OverlappedObject->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	PickupedObject = OverlappedObject;
	OverlappedObject->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	OverlappedObject->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
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

void UPickuperComponent::OnBeginOverlapPickupableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedObject.IsValid()) return;

	OverlappedObject = OtherComp;
}

void UPickuperComponent::OnEndOverlapPickupableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedObject != OtherComp) return;

	OverlappedObject = nullptr;
}
