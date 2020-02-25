// Copyright 2017-2019 MARTA. All Rights Reserved.


#include "ReplicatedSplineMeshComponent.h"

UReplicatedSplineMeshComponent::UReplicatedSplineMeshComponent(const FObjectInitializer& ObjectInitializer)
{
	bReplicates = true;
	SetMobility(EComponentMobility::Movable);
}

void UReplicatedSplineMeshComponent::ApplyPictureSettings(const FPictureSettings& PictureSettings)
{
	SetStaticMesh(PictureSettings.StaticMesh);
	SetCastShadow(PictureSettings.CastShadow);
	SetCollisionProfileName("NoCollision");

	const FVector2D Scale = FVector2D(PictureSettings.Width, PictureSettings.Width);
	SetStartScale(Scale);
	SetEndScale(Scale);
}
