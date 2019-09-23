// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleportComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "NavigationSystem.h"
#include "Engine/StaticMesh.h"
#include "HeadMountedDisplayTypes.h"
#include "TimerManager.h"

UTeleportComponent::UTeleportComponent(const FObjectInitializer& OBJECT_INITIALIZER) : Super(OBJECT_INITIALIZER)
{
	projectileVelocity = 1000.f;
	objectTypeForTeleport = EObjectTypeQuery::ObjectTypeQuery1;

	InitializePathParams();
	InitializeController();
		
	splineComponent = InitializeCustomComponent<USplineComponent>(OBJECT_INITIALIZER, "SplineTrajectory");
	InitializeTeleportLocationComponent(OBJECT_INITIALIZER);

	if (useFade)
	{
		FadeComponent = InitializeCustomComponent<UFadeComponent>(OBJECT_INITIALIZER, "FadeCmponent");
		owner->AddInstanceComponent(FadeComponent.Get());
	}
}

void UTeleportComponent::InitializePathParams()
{
	projectilePathParams.MaxSimTime = 2.5f;
	projectilePathParams.TraceChannel = ECollisionChannel::ECC_WorldStatic;
	projectilePathParams.ObjectTypes = { objectTypeForTeleport };
	projectilePathParams.bTraceComplex = true;
	projectilePathParams.bTraceWithCollision = true;
	projectilePathParams.bTraceWithChannel = false;
	projectilePathParams.ProjectileRadius = 5.f;
}

void UTeleportComponent::InitializeController()
{
	this->bDisplayDeviceModel = true;
	this->bDisableLowLatencyUpdate = true;

	if (trajectoryMesh == nullptr)
	{
		LoadTrajectoryMesh();
	}
}

void UTeleportComponent::LoadTrajectoryMesh()
{
	trajectoryMesh = LoadMesh("StaticMesh'/VRTeleport/SM_TeleportTrajectory.SM_TeleportTrajectory'");
}

template <typename T>
T* UTeleportComponent::InitializeCustomComponent(const FObjectInitializer& OBJECT_INITIALIZER, const FName& COMPONENT_NAME, bool attachToParent)
{
	T* newComponent = OBJECT_INITIALIZER.CreateDefaultSubobject<T>(this, COMPONENT_NAME);

	if (attachToParent)
		newComponent->SetupAttachment(this);

	return newComponent;
}

void UTeleportComponent::InitializeTeleportLocationComponent(const FObjectInitializer& OBJECT_INITIALIZER)
{
	teleportLocationComponent = InitializeCustomComponent<UStaticMeshComponent>(OBJECT_INITIALIZER, "TeleportLocationMesh", false);

	if (teleportLocationMesh == nullptr)
	{
		LoadTeleportLocationMesh();
	}
	
	SetTeleportLocationMesh();
	teleportLocationComponent->SetHiddenInGame(true, true);
	teleportLocationComponent->SetCollisionProfileName("NoCollision");
}

void UTeleportComponent::LoadTeleportLocationMesh()
{
	teleportLocationMesh = LoadMesh("StaticMesh'/VRTeleport/SM_TeleportLocation.SM_TeleportLocation'");
}

UStaticMesh* UTeleportComponent::LoadMesh(const FString& path)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> meshFinder(*path);

	if (meshFinder.Succeeded())
	{
		return meshFinder.Object;
	}

	return nullptr;
}

void UTeleportComponent::SetTeleportLocationMesh() const
{
	teleportLocationComponent->SetStaticMesh(teleportLocationMesh);
}

void UTeleportComponent::InitializeComponent()
{
	//TODO crash
	//SetTeleportLocationMesh();
}

void UTeleportComponent::BeginPlay()
{
	Super::BeginPlay();
	
	projectionTimeline = NewObject<UTimelineComponent>(this, "ProjectionTimeline");
	InitializeTimeline();

	if (autoEnable)
	{
		StartTeleportProjection();
	}

	owner = GetOwner();
	projectilePathParams.ActorsToIgnore.Add(owner.Get());

	if (useFade)
	{
		FadeComponent->OnFadeInFinishedDelegate.AddDynamic(this, &UTeleportComponent::SetActorLocation);
	}
}

void UTeleportComponent::InitializeTimeline()
{
	projectionTimeline->SetLooping(true);
	projectionTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	projectionTimeline->SetTimelineLength(1.f);

	FOnTimelineEvent updateEvent;
	updateEvent.BindUFunction(this, "UpdateProjectionLocation");

	projectionTimeline->SetTimelinePostUpdateFunc(updateEvent);
	
	projectionTimeline->RegisterComponent();
}

void UTeleportComponent::UpdateProjectionLocation()
{
	DestroyTrajectory();
	InitializeTrajectory();

	TArray<FVector> positions;
	GetPathPositions(positions);
	
	if (lastHitResult.bBlockingHit && IsSuitableObjectType())
	{
		SetupSpline(positions);
		DrawTrajectory(positions);
		ShowComponent(teleportLocationComponent.Get());
		SetTeleportLocations();
	} else
	{
		HideComponent(teleportLocationComponent.Get());
	}
}

bool UTeleportComponent::IsSuitableObjectType()
{
	return UEngineTypes::ConvertToObjectType(lastHitResult.Component->GetCollisionObjectType()) == objectTypeForTeleport;
}

void UTeleportComponent::SetupSpline(TArray<FVector>& pathPositions)
{
	splineComponent->SetSplinePoints(pathPositions, ESplineCoordinateSpace::World);
}

void UTeleportComponent::DrawTrajectory(TArray<FVector>& pathPositions)
{
	for (int id = 1; id < pathPositions.Num(); id++)
	{
		CreateSplineMesh(pathPositions, id);
	}
}

void UTeleportComponent::DestroyTrajectory()
{
	for (int id = 0; id < splineMeshes.Num(); id++)
	{
		splineMeshes[id]->DestroyComponent(true);
	}
	splineMeshes.Empty();
}

void UTeleportComponent::InitializeTrajectory()
{
	projectilePathParams.StartLocation = this->GetComponentLocation();
	projectilePathParams.LaunchVelocity = this->GetForwardVector() * projectileVelocity;
}

void UTeleportComponent::CreateSplineMesh(TArray<FVector>& pathPositions, int id)
{
	const FVector CURRENT_LOCATION = pathPositions[id - 1];
	const FVector NEXT_LOCATION = pathPositions[id];

	const FVector START_TANGENT = GetPointTangent(id - 1);
	const FVector END_TANGENT = GetPointTangent(id);

	USplineMeshComponent* splineMesh = NewObject<USplineMeshComponent>(this, *(FString("SplineMeshComponent") + FString::FromInt(id - 1)));
	splineMesh->SetMobility(EComponentMobility::Movable);
	splineMesh->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	splineMesh->SetStaticMesh(trajectoryMesh);
	splineMesh->RegisterComponent();

	splineMesh->SetStartPosition(CURRENT_LOCATION);
	splineMesh->SetEndPosition(NEXT_LOCATION);

	splineMesh->SetStartAndEnd(CURRENT_LOCATION, START_TANGENT, NEXT_LOCATION, END_TANGENT);
	splineMesh->SetCollisionProfileName("NoCollision");

	splineMeshes.Add(splineMesh);
}

void UTeleportComponent::GetPathPositions(TArray<FVector>& targetArray)
{
	FPredictProjectilePathResult projectileResult;
	UGameplayStatics::PredictProjectilePath(this, projectilePathParams, projectileResult);

	TArray<FPredictProjectilePathPointData> pathData = projectileResult.PathData;

	lastHitResult = projectileResult.HitResult;

	targetArray = TArray<FVector>();
	for (FPredictProjectilePathPointData pointData : pathData)
	{
		targetArray.Add(pointData.Location);
	}
}

FVector UTeleportComponent::GetPointTangent(int splinePointId) const
{
	return splineComponent->GetTangentAtSplinePoint(splinePointId, ESplineCoordinateSpace::World);
}

FVector UTeleportComponent::GetLastPointLocation() const
{
	return splineComponent->GetLocationAtSplinePoint(splineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);
}

void UTeleportComponent::ShowComponent(USceneComponent* componentToShow)
{
	SetComponentVisibility(componentToShow, false);
}

void UTeleportComponent::SetComponentVisibility(USceneComponent* component, bool hidden)
{
	component->SetHiddenInGame(hidden, true);
}

void UTeleportComponent::HideComponent(USceneComponent* componentToHide)
{
	SetComponentVisibility(componentToHide, true);
}

void UTeleportComponent::SetTeleportLocations()
{
	if (!useOnlyNavigation)
	{
		lastTeleportLocation = GetLastPointLocation();
		teleportLocationComponent->SetWorldLocation(lastHitResult.Location);
	}

	if (useOnlyNavigation || useNavigation)
	{
		FNavLocation outLocation;
		const bool SUCCEEDED = UNavigationSystemV1::GetNavigationSystem(this)->ProjectPointToNavigation(lastHitResult.Location, outLocation, queryExtent);

		if (SUCCEEDED)
		{
			lastTeleportLocation = outLocation;
			teleportLocationComponent->SetWorldLocation(outLocation);
		}
	}
	
	teleportLocationComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void UTeleportComponent::StartTeleportProjection()
{
	if (projectionTimeline.IsValid())
	{
		projectionTimeline->PlayFromStart();
		ShowComponent(teleportLocationComponent.Get());
	} else
	{
		checkNoEntry();
	}
}

void UTeleportComponent::Teleport()
{
	StopTeleportProjection();
	DestroyTrajectory();
	if (useFade)
	{
		FadeComponent->StartFade();
	} else
	{
		SetActorLocation();
	}
}

void UTeleportComponent::StopTeleportProjection()
{
	if (projectionTimeline.IsValid())
	{
		projectionTimeline->Stop();
		HideComponent(teleportLocationComponent.Get());
	}
	else
	{
		checkNoEntry();
	}
}

void UTeleportComponent::SetActorLocation()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0, EOrientPositionSelector::Position);
	owner->SetActorLocation(CalculateLocation());
}

FVector UTeleportComponent::CalculateLocation()
{
	if (lastHitResult.bBlockingHit && IsSuitableObjectType())
	{
		const FVector OWNER_LOCATION = owner->GetActorLocation();

		const FVector RESULT_LOCATION = FVector(
			lastTeleportLocation.X,
			lastTeleportLocation.Y,
			lastTeleportLocation.Z + OWNER_LOCATION.Z
		);

		return RESULT_LOCATION;
	}

	return owner->GetActorLocation();
}
