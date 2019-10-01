// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserPointer.h"
#include "Kismet/KismetMathLibrary.h"

ULaserPointer::ULaserPointer()
{
	PrimaryComponentTick.bCanEverTick = true;
	CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	SetMobility(EComponentMobility::Movable);
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

	for (TEnumAsByte<ECollisionChannel> ObjectChannel : ObjectChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(ObjectChannel);
	}

	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);
	CollisionQueryParams.bTraceComplex = true;
}

void ULaserPointer::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	const FVector Start = GetComponentLocation();
	const FVector End = Distance * GetForwardVector() + Start;
	
	GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, CollisionQueryParams);

	SetStartPosition(FVector::ZeroVector);
	
	const FVector WorldEndPosition = HitResult.bBlockingHit ? HitResult.Location : End;
	const FVector LocalEndPosition = UKismetMathLibrary::InverseTransformLocation(GetComponentTransform(), WorldEndPosition);
	SetEndPosition(LocalEndPosition);
}

void ULaserPointer::SetEnabled(bool bIsEnabled)
{
	SetHiddenInGame(!bIsEnabled, true);
}
