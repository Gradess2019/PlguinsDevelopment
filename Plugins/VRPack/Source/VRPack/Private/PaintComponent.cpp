// Copyright 2017-2019 MARTA. All Rights Reserved.

#include "PaintComponent.h"
#include "Components/TimelineComponent.h"

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

	DrawingTimeline->RegisterComponent();
}

void UPaintComponent::StartDrawing()
{
#if !UE_BUILD_SHIPPING
	if (!DrawingTimeline.IsValid()) checkNoEntry();
#endif

	DrawingTimeline->PlayFromStart();
}

void UPaintComponent::StopDrawing()
{
#if !UE_BUILD_SHIPPING
	if (!DrawingTimeline.IsValid()) checkNoEntry();
#endif

	DrawingTimeline->Stop();
}
