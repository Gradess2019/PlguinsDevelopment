// Copyright 2017-2019 MARTA. All Rights Reserved.


#include "AttachableStaticMeshComponent.h"

bool UAttachableStaticMeshComponent::OnAttach_Implementation(USceneComponent* Parent)
{
	OnAttached.Broadcast(Parent);
	return true;
}

void UAttachableStaticMeshComponent::OnDetach_Implementation()
{
	OnDetached.Broadcast();
}

