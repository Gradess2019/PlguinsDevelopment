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
	TArray<UPrimitiveComponent*> OverlappingComponents;

	this->GetOverlappingActors(OverlappingActors);
	this->GetOverlappingComponents(OverlappingComponents);

	if (OverlappingActors.Num() == 0 && OverlappingComponents.Num() == 0) { return; }

	TArray<UObject*> OverlappingObjects;

	UVRPackFunctionLibrary::AppendArray<UObject, UPrimitiveComponent>(OverlappingObjects, OverlappingComponents);
	UVRPackFunctionLibrary::AppendArray<UObject, AActor>(OverlappingObjects, OverlappingActors);

	if (OverlappingObjects[0]->Implements<UPickupableObject>())
	{
		PickupedObject = OverlappingObjects[0];
		IPickupableObject::Execute_OnAttach(PickupedObject.Get(), this);
	}
}

void UPickuperComponent::Throw()
{
	if (!PickupedObject.IsValid()) return;
	IPickupableObject::Execute_OnDetach(PickupedObject.Get());
	PickupedObject = nullptr;
}

