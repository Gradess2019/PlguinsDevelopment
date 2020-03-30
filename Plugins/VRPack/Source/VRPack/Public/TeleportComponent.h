// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineComponent.h"
#include "TimerManager.h"
#include "TeleportComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartProjection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopProjection);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VRPACK_API UTeleportComponent : public UMotionControllerComponent
{
	GENERATED_BODY()

public:

	UTeleportComponent(const FObjectInitializer& OBJECT_INITIALIZER);
	
	UFUNCTION(BlueprintCallable, Category = "Teleport settings")
	void StartTeleportProjection();

	UFUNCTION(BlueprintCallable, Category = "Teleport settings")
	void Teleport();

	UFUNCTION(BlueprintCallable, Category = "Teleport settings")
	void StopTeleportProjection();

	/*
	 * Used for binding to delegates
	 */
	UFUNCTION(BlueprintCallable, Category = "Teleport settings")
	void SetOwnerLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintAssignable, Category = "Teleport settings")
	FOnTeleportation OnTeleportation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable, Category = "Teleport settings")
	FOnStartProjection OnStartProjection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable, Category = "Teleport settings")
	FOnStopProjection OnStopProjection;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	UStaticMesh* teleportLocationMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	UStaticMesh* trajectoryMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	bool autoEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	bool useFade;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	bool useOnlyNavigation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	bool useNavigation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings", meta = (EditCondition = "useOnlyNavigation"))
	FVector queryExtent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	FPredictProjectilePathParams projectilePathParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	float projectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Teleport settings")
	TEnumAsByte<EObjectTypeQuery> objectTypeForTeleport;

	void BeginPlay() override;

private:

	FHitResult& lastHitResult = *new FHitResult(ENoInit::NoInit);

	UPROPERTY()
	TWeakObjectPtr<UTimelineComponent> projectionTimeline;

	UPROPERTY()
	TArray<USplineMeshComponent*> splineMeshes;

	UPROPERTY()
	FVector lastTeleportLocation;

	UPROPERTY()
	TWeakObjectPtr<AActor> owner;

	UPROPERTY()
	TWeakObjectPtr<USplineComponent> splineComponent;

	UPROPERTY()
	TWeakObjectPtr<UStaticMeshComponent> teleportLocationComponent;

	int uniqueId;

	FVector OwnerExtent;

	FTimerHandle TeleportHandle;
	FTimerDelegate TeleportDelegate;
	
	void InitializePathParams();
	void InitializeController();
	void LoadTrajectoryMesh();

	template<typename T>
	T* InitializeCustomComponent(const FObjectInitializer& OBJECT_INITIALIZER, const FName& COMPONENT_NAME, bool attachToParent = true);

	void InitializeTeleportLocationComponent();
	void LoadTeleportLocationMesh();
	UStaticMesh* LoadMesh(const FString& path);
	void SetTeleportLocationMesh() const;

	void InitializeComponent() override;
	void InitializeTimeline();
	
	UFUNCTION()
	void UpdateProjectionLocation();
	bool IsSuitableObjectType();

	UFUNCTION()
	void SetupSpline(TArray<FVector>& pathPositions);

	UFUNCTION()
	void DrawTrajectory(TArray<FVector>& pathPositions);

	UFUNCTION()
	void CreateSplineMesh(TArray<FVector>& pathPositions, int id);
	FVector GetPointTangent(int splinePointId) const;
	FVector GetLastPointLocation() const;
	void ShowComponent(USceneComponent* componentToShow);

	UFUNCTION()
	void SetComponentVisibility(USceneComponent* component, bool hidden);

	UFUNCTION()
	void HideComponent(USceneComponent* componentToHide);

	UFUNCTION()
	void SetTeleportLocations();
	
	void DestroyTrajectory();
	void InitializeTrajectory();

	UFUNCTION()
	void GetPathPositions(TArray<FVector>& targetArray);

	FVector CalculateLocation();
};

