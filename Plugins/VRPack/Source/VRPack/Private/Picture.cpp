// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "Picture.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APicture::APicture(const FObjectInitializer& ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "RootSceneComponent");
}

void APicture::InitializePicture(FPictureSettings PictureSettings)
{
	this->PictureSettings = PictureSettings;
}

void APicture::Follow(FVector PointLocation)
{
	if (!IsAllowableSize() || !IsAllowableAngle())
	{
		CalculateSplineTangentAndPositions(PointLocation);
		CreateNewMesh(PointLocation);
	}
	CalculateSplineTangentAndPositions(PointLocation);
}

bool APicture::IsAllowableSize() const
{
	return (CurrentSlice->GetEndPosition() - CurrentSlice->GetStartPosition()).Size() < PictureSettings.SliceSize;
}

bool APicture::IsAllowableAngle() const
{
	if (!LastDrawnSlice.IsValid()) return false;
	if (CurrentSlice->GetEndPosition().Equals(FVector::ZeroVector, 1.f)) return true;

	const FVector LastSplineDirection = UKismetMathLibrary::Normal(LastDrawnSlice->GetEndPosition() - LastDrawnSlice->GetStartPosition());
	const FVector CurrentSplineDirection = UKismetMathLibrary::Normal(CurrentSlice->GetEndPosition() - CurrentSlice->GetStartPosition());
	const float Angle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(CurrentSplineDirection, LastSplineDirection));
	return PictureSettings.AllowableAngle > Angle;
}

void APicture::CalculateSplineTangentAndPositions(FVector PointLocation) const
{
	const FVector EndPosition = UKismetMathLibrary::InverseTransformLocation(CurrentSlice->GetComponentTransform(), PointLocation);
	const FVector EndTangent = CurrentSlice->GetEndPosition() - CurrentSlice->GetStartPosition();

	if (!LastDrawnSlice.IsValid())
	{
		CurrentSlice->SetStartTangent(EndTangent);
	}

	CurrentSlice->SetEndTangent(EndTangent);
	CurrentSlice->SetEndPosition(EndPosition);
}

void APicture::CreateNewMesh(FVector PointLocation)
{
	if (CurrentSlice.IsValid())
	{
		const FVector LocalPointPostition = UKismetMathLibrary::InverseTransformLocation(CurrentSlice->GetComponentTransform(), PointLocation);
		LastDrawnSlice = CurrentSlice;
		CreateSplineMeshComponent(LastDrawnSlice->GetEndPosition() + LastDrawnSlice->RelativeLocation);
		CurrentSlice->SetStartAndEnd(FVector::ZeroVector, LastDrawnSlice->GetEndTangent(), LocalPointPostition, LocalPointPostition);
	} else
	{
		const FVector LocalPointPostition = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PointLocation);
		CreateSplineMeshComponent(LocalPointPostition);
		CurrentSlice->SetStartAndEnd(FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector, FVector::ZeroVector);
	}
	
	CurrentSlice->SetStaticMesh(PictureSettings.StaticMesh);
}

void APicture::CreateSplineMeshComponent(FVector RelativeLocation)
{
	CurrentSlice = NewObject<USplineMeshComponent>(this);
	CurrentSlice->SetMobility(EComponentMobility::Movable);
	CurrentSlice->RegisterComponent();
	CurrentSlice->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	CurrentSlice->SetRelativeLocation(RelativeLocation);
}

void APicture::FinishFollowing()
{
	LastDrawnSlice = nullptr;
	CurrentSlice = nullptr;
}

