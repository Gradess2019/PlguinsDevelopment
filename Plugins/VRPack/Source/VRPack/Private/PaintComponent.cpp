// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PaintComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

UPaintComponent::UPaintComponent(const FObjectInitializer& ObjectInitializer)
{
	bWantsInitializeComponent = true;
	TimelineSettings.Loop = true;
}

void UPaintComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	InitializeDrawingTimeline();
}

void UPaintComponent::InitializeDrawingTimeline()
{
	DrawingTimeline = NewObject<UTimelineComponent>(this, "DrawingTimeline");

	DrawingTimeline->SetLooping(TimelineSettings.Loop);
	DrawingTimeline->SetIgnoreTimeDilation(TimelineSettings.IgnoreTimeDilation);

	DrawingTimeline->SetTimelineLength(TimelineSettings.Length);
	DrawingTimeline->SetPlayRate(TimelineSettings.PlayRate);
	
	DrawingTimeline->SetTimelineLengthMode(TimelineSettings.Mode);

	FOnTimelineEvent TimelineEvent;
	TimelineEvent.BindUFunction(this, "OnUpdatePaint");
	DrawingTimeline->SetTimelinePostUpdateFunc(TimelineEvent);

	DrawingTimeline->RegisterComponent();
}

void UPaintComponent::OnUpdatePaint()
{
	CurrentPicture->Follow(this->GetComponentLocation());
}

void UPaintComponent::StartDrawing_Implementation()
{
#if !UE_BUILD_SHIPPING
	if (!IsValid(DrawingTimeline)) checkNoEntry();
#endif

	if (!IsValid(CurrentPicture))
	{
		FActorSpawnParameters Parameters;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Parameters.Owner = GetOwner();
		CurrentPicture = GetWorld()->SpawnActor<APicture>(PictureClass, Parameters);
		CurrentPicture->SetActorLocation(GetComponentLocation());
		CurrentPicture->InitializePicture(PictureSettings);
	}

	CurrentPicture->CreateNewMesh(this->GetComponentLocation());

	
#if !UE_BUILD_SHIPPING
	if (!IsValid(CurrentPicture)) checkNoEntry();
#endif
	
	DrawingTimeline->PlayFromStart();
}

void UPaintComponent::StopDrawing()
{
#if !UE_BUILD_SHIPPING
	if (!IsValid(DrawingTimeline)) checkNoEntry();
#endif

	DrawingTimeline->Stop();

	if (!IsValid(CurrentPicture)) { return; }
	CurrentPicture->FinishFollowing();
}

void UPaintComponent::FinishDrawing()
{
	DrawingTimeline->Stop();

	if (!IsValid(CurrentPicture)) { return; }
	CurrentPicture->EnableCollision();
	CurrentPicture = nullptr;

	OnFinsihDrawing.Broadcast();
}

void UPaintComponent::SetLineWidth(float NewWidth)
{
	PictureSettings.Width = NewWidth;
	UpdatePictureSettings();
}

// TODO may Observer design pattern would be better?
void UPaintComponent::UpdatePictureSettings() const
{
	if (!IsValid(CurrentPicture)) { return; }
	CurrentPicture->UpdatePictureSettings(PictureSettings);
}

void UPaintComponent::SetLineColor(const FLinearColor& NewColor)
{
	PictureSettings.Color = NewColor;
	UpdatePictureSettings();
}

APicture* UPaintComponent::GetCurrentPicture()
{
	return CurrentPicture;
}

bool UPaintComponent::IsDrawing()
{
	return DrawingTimeline->IsPlaying();
}
