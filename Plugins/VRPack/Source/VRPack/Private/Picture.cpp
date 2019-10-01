// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "Picture.h"
#include "Kismet/KismetMathLibrary.h"

APicture::APicture(const FObjectInitializer& ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "RootSceneComponent");
}

void APicture::InitializePicture(float PictureSliceSize, UStaticMesh* PictureMesh)
{
	this->SliceSize = PictureSliceSize;
	this->Mesh = PictureMesh;
}

void APicture::Follow(FVector PointLocation)
{
	if ((CurrentSlice->GetEndPosition() - CurrentSlice->GetStartPosition()).Size() > SliceSize)
	{
		CreateNewMesh(PointLocation);
	}
	
	FVector EndPosition = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PointLocation);

	CurrentSlice->SetStartAndEnd(CurrentSlice->GetStartPosition(), EndPosition, EndPosition, EndPosition);
	
}

void APicture::CreateNewMesh(FVector PointLocation)
{
	FVector StartPosition = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PointLocation);
	CurrentSlice = NewObject<USplineMeshComponent>(this);
	CurrentSlice->SetMobility(EComponentMobility::Movable);
	CurrentSlice->RegisterComponent();
	CurrentSlice->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	CurrentSlice->SetStartAndEnd(StartPosition, StartPosition, StartPosition, StartPosition);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *StartPosition.ToString());
	//CurrentSlice->SetWorldLocation(PointLocation);
	CurrentSlice->SetStaticMesh(Mesh);
}

void APicture::DrawNext(FVector PointLocation)
{
	CreateNewMesh(CurrentSlice->GetEndPosition());
}
