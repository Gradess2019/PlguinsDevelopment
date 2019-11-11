// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PickuperComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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
	TArray<UStaticMeshComponent*> Components;
	PickupedActor->GetComponents<UStaticMeshComponent>(Components);

	for (UStaticMeshComponent* Component : Components)
	{
		FStaticMeshData Data(Component);

		StaticMeshDatas.Add(Data);

		Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		Component->SetCollisionResponseToChannel(GetCollisionObjectType(), ECollisionResponse::ECR_Ignore);
	}
	
	PickupedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
}

void UPickuperComponent::Throw()
{
	if (!PickupedActor.IsValid()) return;

	for (FStaticMeshData& Data : StaticMeshDatas)
	{
		Data.ApplySavedData();
	}
	StaticMeshDatas.Empty();

	PickupedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickupedActor = nullptr;
}

