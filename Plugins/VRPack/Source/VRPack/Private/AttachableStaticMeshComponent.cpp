// Copyright 2017-2019 MARTA. All Rights Reserved.


#include "AttachableStaticMeshComponent.h"

void UAttachableStaticMeshComponent::OnAttach_Implementation(USceneComponent* Parent)
{
	OnAttached.Broadcast(Parent);
}

void UAttachableStaticMeshComponent::OnDetach_Implementation()
{
	OnDetached.Broadcast();
}

