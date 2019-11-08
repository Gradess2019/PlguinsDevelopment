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

void APicture::FinishFollowing()
{
	LastDrawnSlice = nullptr;
	CurrentSlice = nullptr;
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
	if (CurrentSlice->GetEndPosition().Equals(FVector::ZeroVector, 0.1f)) return true;

	return PictureSettings.AllowableAngle > GetAngle();
}

float APicture::GetAngle() const
{
	const FVector LastSplineDirection = UKismetMathLibrary::Normal(
		LastDrawnSlice->GetEndPosition() - LastDrawnSlice->GetStartPosition());
	const FVector CurrentSplineDirection = UKismetMathLibrary::Normal(
		CurrentSlice->GetEndPosition() - CurrentSlice->GetStartPosition());

	const float DotProduct = UKismetMathLibrary::Dot_VectorVector(CurrentSplineDirection, LastSplineDirection);
	return UKismetMathLibrary::DegAcos(DotProduct);
}

void APicture::CalculateSplineTangentAndPositions(FVector PointLocation) const
{
	const FVector EndPosition = UKismetMathLibrary::InverseTransformLocation(
		CurrentSlice->GetComponentTransform(), PointLocation);
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
	const FTransform ParentTransform = GetParentTransform();
	const FVector LocalPointPostition = UKismetMathLibrary::InverseTransformLocation(ParentTransform, PointLocation);

	FSplineMeshInitializer Initializer;
	FVector RelativeLocation;

	if (CurrentSlice.IsValid())
	{
		RelativeLocation = CurrentSlice->GetEndPosition() + CurrentSlice->RelativeLocation;
		Initializer = FSplineMeshInitializer(FVector::ZeroVector, CurrentSlice->GetEndTangent(), LocalPointPostition,
		                                     LocalPointPostition);
		LastDrawnSlice = CurrentSlice;
	}
	else
	{
		RelativeLocation = LocalPointPostition;
	}

	CreateSplineMeshComponent(RelativeLocation);
	SetStartAndEnd(Initializer);
	UE_LOG(LogTemp, Warning, TEXT("New mesh was created"));
}

FTransform APicture::GetParentTransform() const
{
	return CurrentSlice.IsValid()
		       ? CurrentSlice->GetComponentTransform()
		       : GetActorTransform();
}

void APicture::CreateSplineMeshComponent(FVector RelativeLocation)
{
	CurrentSlice = NewObject<USplineMeshComponent>(this);
	CurrentSlice->SetMobility(EComponentMobility::Movable);
	CurrentSlice->RegisterComponent();
	CurrentSlice->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	CurrentSlice->SetRelativeLocation(RelativeLocation);
	CurrentSlice->SetStaticMesh(PictureSettings.StaticMesh);
}

void APicture::SetStartAndEnd(const FSplineMeshInitializer& Initializer) const
{
	CurrentSlice->SetStartAndEnd(Initializer.StartPos, Initializer.StartTangent, Initializer.EndPos,
	                             Initializer.EndTangent);
}
