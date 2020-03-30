// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PickupableObject.h"
#include "VRPackFunctionLibrary.h"
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
	TArray<UPrimitiveComponent*> CurrentOverlappingComponents;

	this->GetOverlappingActors(OverlappingActors);
	this->GetOverlappingComponents(CurrentOverlappingComponents);

	if (OverlappingActors.Num() == 0 && CurrentOverlappingComponents.Num() == 0) { return; }

	TArray<UObject*> OverlappingObjects;

	UVRPackFunctionLibrary::AppendArray<UObject, UPrimitiveComponent>(OverlappingObjects, CurrentOverlappingComponents);
	UVRPackFunctionLibrary::AppendArray<UObject, AActor>(OverlappingObjects, OverlappingActors);

	for (UObject* Element : OverlappingObjects)
	{
		if (!Element->Implements<UPickupableObject>()) { continue; }

		if (IPickupableObject::Execute_OnAttach(Element, this))
		{
			PickupedObject = Element;
			return;
		}
	}
}

void UPickuperComponent::Throw()
{
	if (!PickupedObject.IsValid()) return;
	IPickupableObject::Execute_OnDetach(PickupedObject.Get());
	IInvalidator::Execute_Invalidate(this);
}

void UPickuperComponent::Invalidate_Implementation()
{
	PickupedObject = nullptr;
}

