#include "HeightChangerComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

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
	const FVector ComponentLocation = TargetComponent->RelativeLocation;
	const FVector TargetLocation = FVector(ComponentLocation.X, ComponentLocation.Y, NewHeight);
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

float UHeightChangerComponent::GetHeight()
{
	return CurrentHeight;
}

void UHeightChangerComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetComponent = Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(TargetComponentName));

	if (TargetComponent.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Component valid"));
		CurrentHeight = TargetComponent->RelativeLocation.Z;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Component INVALID!"));
	}
}
