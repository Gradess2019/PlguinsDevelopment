// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineComponent.h"
#include "FadeComponent.h"
#include "TimerManager.h"
#include "TeleportComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTELEPORT_API UTeleportComponent : public UMotionControllerComponent
{
	GENERATED_BODY()

public:

	UTeleportComponent(const FObjectInitializer& OBJECT_INITIALIZER);
	
	UFUNCTION(BlueprintCallable)
	void StartTeleportProjection();

	UFUNCTION(BlueprintCallable)
	void Teleport();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	UStaticMesh* teleportLocationMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	UStaticMesh* trajectoryMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	bool autoEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	bool useFade;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	bool useOnlyNavigation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	bool useNavigation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings", meta = (EditCondition = "useOnlyNavigation"))
	FVector queryExtent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	FPredictProjectilePathParams projectilePathParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
	float projectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component settings")
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
	UStaticMeshComponent* teleportLocationComponent;

	UPROPERTY()
	TWeakObjectPtr<UFadeComponent> FadeComponent;

	FTimerHandle TeleportHandle;
	FTimerDelegate TeleportDelegate;
	
	void InitializePathParams();
	void InitializeController();
	void LoadTrajectoryMesh();

	template<typename T>
	T* InitializeCustomComponent(const FObjectInitializer& OBJECT_INITIALIZER, const FName& COMPONENT_NAME, bool attachToParent = true);

	void InitializeTeleportLocationComponent(const FObjectInitializer& OBJECT_INITIALIZER);
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

	UFUNCTION()
	void StopTeleportProjection();

	UFUNCTION()
	void SetOwnerLocation();

	FVector CalculateLocation();

	bool IsFadePlaying();
};

