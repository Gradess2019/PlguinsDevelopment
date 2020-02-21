// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "Picture.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/SplineComponent.h"
#include "Engine/StaticMesh.h"
#include "Net/UnrealNetwork.h"

#define LOCAL_SPACE ESplineCoordinateSpace::Local
#define WORLD_SPACE ESplineCoordinateSpace::World

APicture::APicture(const FObjectInitializer& ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "RootSceneComponent");
	
	bReplicates = true;
}

void APicture::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(Spline)) { return; }
	
	Spline = NewObject<USplineComponent>(this, "SplineComponent");
	Spline->RegisterComponent();
	Spline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Spline->SetRelativeLocation(FVector::ZeroVector);
}

void APicture::InitializePicture_Implementation(FPictureSettings PictureSettings)
{
	this->PictureSettings = PictureSettings;

	Spline->SetLocationAtSplinePoint(1, FVector::ZeroVector, LOCAL_SPACE);
	Spline->SetSplinePointType(1, ESplinePointType::Linear);
}

void APicture::UpdatePictureSettings(FPictureSettings PictureSettings)
{
	this->PictureSettings = PictureSettings;
	CreateMaterial();
}

void APicture::EnableCollision()
{
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);

	for (UStaticMeshComponent* Component : Components)
	{
		Component->SetCollisionProfileName(PictureSettings.CollisionPreset);
	}
}

void APicture::FinishFollowing()
{
	CurrentSlice = nullptr;
}

void APicture::Follow(FVector PointLocation)
{
	Spline->SetLocationAtSplinePoint(GetCurrentPointIndex(), PointLocation, LOCAL_SPACE);
	if (!IsAllowableSize() || !IsAllowableAngle())
	{
		CreateNewMesh(PointLocation);
	} else if (CurrentSlice.IsValid())
	{
		const FVector NewEndLocation = Spline->GetLocationAtSplinePoint(GetCurrentPointIndex(), LOCAL_SPACE);
		CurrentSlice->SetEndPosition(NewEndLocation);
	}
}

bool APicture::IsAllowableSize() const
{
	const FVector PreviousLocation = Spline->GetLocationAtSplinePoint(GetPreviousPointIndex(), LOCAL_SPACE);
	const FVector CurrentLocation = Spline->GetLocationAtSplinePoint(GetCurrentPointIndex(), LOCAL_SPACE);
	return (PreviousLocation - CurrentLocation).Size() < PictureSettings.SliceSize;
}

bool APicture::IsAllowableAngle() const
{
	if (CurrentSlice->GetEndPosition().Equals(FVector::ZeroVector, PictureSettings.SliceAngleTolerance)) return true;

	return PictureSettings.AllowableAngle > GetAngle();
}

int32 APicture::GetPreviousPointIndex() const
{
	return GetCurrentPointIndex() - 1;
}

float APicture::GetAngle() const
{
	const FVector PreviousSplineDirection = UKismetMathLibrary::Normal(
		Spline->GetDirectionAtSplinePoint(GetPreviousPointIndex(), WORLD_SPACE), 
		PictureSettings.SliceAngleTolerance
	);

	const FVector CurrentSplineDirection = UKismetMathLibrary::Normal(
		CurrentSlice->GetStartTangent(),
		PictureSettings.SliceAngleTolerance
	);

	const float DotProduct = UKismetMathLibrary::Dot_VectorVector(PreviousSplineDirection, CurrentSplineDirection);
	return UKismetMathLibrary::DegAcos(DotProduct);
}

int32 APicture::GetCurrentPointIndex() const
{
	return Spline->GetNumberOfSplinePoints() - 1;
}

void APicture::CreateNewMesh(FVector PointLocation)
{
	if (CurrentSlice.IsValid())
	{
		const FVector EndPosition = Spline->GetLocationAtSplinePoint(GetCurrentPointIndex(), LOCAL_SPACE);
		CurrentSlice->SetEndPosition(EndPosition, false);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(GetPreviousPointIndex(), WORLD_SPACE);
		CurrentSlice->SetEndTangent(EndTangent);
	} else
	{
		Spline->AddSplinePoint(PointLocation, LOCAL_SPACE);
		Spline->AddSplinePoint(PointLocation, LOCAL_SPACE);
	}

	CreateSplineMeshComponent();

	FVector InitialPosition = Spline->GetLocationAtSplinePoint(GetCurrentPointIndex(), LOCAL_SPACE);
	FVector InitialTangent = Spline->GetTangentAtSplinePoint(GetPreviousPointIndex(), WORLD_SPACE);
	FSplineMeshInitializer Initializer = FSplineMeshInitializer(
		InitialPosition,
		InitialTangent,
		InitialPosition,
		InitialTangent
	);

	SetStartAndEnd(Initializer);

	Spline->AddSplinePoint(PointLocation, LOCAL_SPACE);
	Spline->SetSplinePointType(GetCurrentPointIndex(), ESplinePointType::Linear);
}

FTransform APicture::GetParentTransform() const
{
	return CurrentSlice.IsValid()
		       ? CurrentSlice->GetComponentTransform()
		       : GetActorTransform();
}

void APicture::CreateSplineMeshComponent()
{
	CurrentSlice = NewObject<USplineMeshComponent>(this);
	CurrentSlice->SetMobility(EComponentMobility::Movable);
	CurrentSlice->RegisterComponent();
	CurrentSlice->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	CurrentSlice->SetStaticMesh(PictureSettings.StaticMesh);
	CurrentSlice->SetCastShadow(PictureSettings.CastShadow);
	CurrentSlice->SetCollisionProfileName("NoCollision");

	const FVector2D Scale = FVector2D(PictureSettings.Width, PictureSettings.Width);
	CurrentSlice->SetStartScale(Scale);
	CurrentSlice->SetEndScale(Scale);

	SetMaterial();
}

void APicture::SetMaterial()
{
	if (!Material.IsValid())
	{
		CreateMaterial();
	}
	CurrentSlice->SetMaterial(0, Material.Get());
}

void APicture::CreateMaterial()
{
	if (!IsValid(PictureSettings.StaticMesh)) { return; }
	UMaterialInterface* SourceMaterial = PictureSettings.StaticMesh->GetMaterial(0);
	Material = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, SourceMaterial);
	Material->SetVectorParameterValue("Color", PictureSettings.Color);
}

void APicture::SetStartAndEnd(const FSplineMeshInitializer& Initializer) const
{
	CurrentSlice->SetStartAndEnd(
		Initializer.StartPos, 
		Initializer.StartTangent, 
		Initializer.EndPos,
		Initializer.EndTangent
	);
}

void APicture::SetVisible(bool bVisible)
{
	this->SetActorHiddenInGame(!bVisible);
}

bool APicture::OnAttach_Implementation(USceneComponent* Parent)
{
	AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
	return true;
}

void APicture::OnDetach_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}



void APicture::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APicture, PictureSettings);
}
