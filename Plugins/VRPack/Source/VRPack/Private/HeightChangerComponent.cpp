#include "HeightChangerComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UHeightChangerComponent::UHeightChangerComponent()
{
	HeightCoefficient = 100.f;
	Increment = 0.25f;
	Decrement = 0.25f;
	MaxHeight = 200.f;
	MinHeight = 130.f;
}

void UHeightChangerComponent::Raise()
{
	SetCustomHeight(CurrentHeight + Increment);
}

void UHeightChangerComponent::Lower()
{
	SetCustomHeight(CurrentHeight - Decrement);
}

void UHeightChangerComponent::SetCustomHeight(float NewHeight)
{
	NewHeight = UKismetMathLibrary::FClamp(NewHeight, MinHeight, MaxHeight);

	const FVector ComponentLocation = TargetComponent->RelativeLocation;
	const FVector TargetLocation = FVector(ComponentLocation.X, ComponentLocation.Y, NewHeight - HeightCoefficient);

	TargetComponent->SetRelativeLocation(TargetLocation);
	CurrentHeight = NewHeight;
}

void UHeightChangerComponent::SetIncrement(float NewIncrement)
{
	Increment = NewIncrement;
}

void UHeightChangerComponent::SetDecrement(const float NewDecrement)
{
	Decrement = NewDecrement;
}

float UHeightChangerComponent::GetHeight() const
{
	return CurrentHeight;
}

float UHeightChangerComponent::GetMaxHeight() const
{
	return MaxHeight;
}

float UHeightChangerComponent::GetMinHeight() const
{
	return MinHeight;
}

void UHeightChangerComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetComponent = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TargetComponentName));

	if (TargetComponent.IsValid())
	{
		SetCustomHeight(TargetComponent->RelativeLocation.Z + HeightCoefficient);
	} else
	{
		checkNoEntry();
	}
	
}
