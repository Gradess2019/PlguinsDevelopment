// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

UPickuperComponent::UPickuperComponent()
{
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxExtent = FVector(10.f);
}

void UPickuperComponent::Pickup()
{
	TArray<AActor*> OverlappingActors;
	this->GetOverlappingActors(OverlappingActors);
	
	if (OverlappingActors.Num() == 0) { return; }

	PickupedActor = OverlappingActors[0];
	PickupedActor->SetSimulatePhysics(false);
	PickupedActor->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PickupedActor->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
	PickupedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

}

void UPickuperComponent::Throw()
{
	if (!PickupedActor.IsValid()) return;

	PickupedActor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	PickupedActor->SetSimulatePhysics(true);
	PickupedActor->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	PickupedActor->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Block);
	PickupedActor = nullptr;
}

