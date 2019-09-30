// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPointer.h"

ULaserPointer::ULaserPointer()
{
	CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
}

void ULaserPointer::EnableLaser()
{
	SetEnabled(true);
}

void ULaserPointer::DisableLaser()
{
	SetEnabled(false);
}

void ULaserPointer::BeginPlay()
{
	Super::BeginPlay();

	for (TEnumAsByte<ECollisionChannel> ObjectChannel : objectChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(ObjectChannel);
	}

	
}

void ULaserPointer::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	const FVector Start = GetComponentLocation();
	const FVector End = Distance * GetForwardVector();
	
	GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, CollisionQueryParams);
}

void ULaserPointer::SetEnabled(bool bIsEnabled)
{
	SetHiddenInGame(bIsEnabled, true);
}
