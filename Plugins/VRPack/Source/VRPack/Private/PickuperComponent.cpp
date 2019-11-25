// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PickupableObject.h"
#include "GameFramework/Actor.h"

UPickuperComponent::UPickuperComponent()
{
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxExtent = FVector(10.f);
	StaticMeshDatas = TArray<FStaticMeshData>();
}

void UPickuperComponent::Pickup()
{
	TArray<AActor*> OverlappingActors;
	this->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() == 0) { return; }

	PickupedActor = OverlappingActors[0];
	if (PickupedActor->Implements<UPickupableObject>())
	{
		IPickupableObject::Execute_OnAttach(PickupedActor.Get(), this);
	} else
	{
		PickupedActor = nullptr;
	}
}

void UPickuperComponent::Throw()
{
	if (!PickupedActor.IsValid()) return;
	IPickupableObject::Execute_OnDetach(PickupedActor.Get());
	PickupedActor = nullptr;
}

