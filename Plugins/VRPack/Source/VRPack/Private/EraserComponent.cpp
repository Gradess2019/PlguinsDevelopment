// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "EraserComponent.h"

void UEraserComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UEraserComponent::OnEraserBeginOverlap);
}

void UEraserComponent::OnEraserBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ClassToErase)) { return; }
	OtherComp->DestroyComponent(true);
}
