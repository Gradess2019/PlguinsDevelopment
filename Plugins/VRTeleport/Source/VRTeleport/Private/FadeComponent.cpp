#include "FadeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"

UFadeComponent::UFadeComponent()
{
	bWantsBeginPlay = true;
}

void UFadeComponent::BeginPlay()
{
	Super::BeginPlay();
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, PlayerID);
}

void UFadeComponent::StartFade()
{
	if (!CameraManager.IsValid()) checkNoEntry();

	FadeIn();
	GetWorld()->GetTimerManager().SetTimer(FullFade, this, &UFadeComponent::FadeOut, FadeInSettings.Duration);
}

void UFadeComponent::StopFade()
{
	CameraManager->StopCameraFade();
	ClearTimers();
}

void UFadeComponent::ClearTimers()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UFadeComponent::FadeIn()
{
	FullFade.Invalidate();
	FadeOutFinishedTimer.Invalidate();
	
	Fade(FadeInSettings);
	OnFadeInStartedDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(FadeInFinishedTimer, this, &UFadeComponent::OnFadeInFinished, FadeInSettings.Duration);
}

void UFadeComponent::FadeOut()
{
	FullFade.Invalidate();
	FadeInFinishedTimer.Invalidate();

	Fade(FadeOutSettings);
	OnFadeOutStartedDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(FadeInFinishedTimer, this, &UFadeComponent::OnFadeOutFinished, FadeOutSettings.Duration);
}

bool UFadeComponent::IsPlaying()
{
	return FullFade.IsValid() || FadeInFinishedTimer.IsValid() || FadeOutFinishedTimer.IsValid();
}

void UFadeComponent::Fade(const FFadeSettings& FADE_SETTINGS)
{
	CameraManager->StartCameraFade(
		FADE_SETTINGS.FromAlpha,
		FADE_SETTINGS.ToAlpha,
		FADE_SETTINGS.Duration,
		FADE_SETTINGS.Color,
		FADE_SETTINGS.bShouldFadeAudio,
		FADE_SETTINGS.bHoldWhenFinished
	);
}

void UFadeComponent::OnFadeInFinished()
{
	OnFadeInFinishedDelegate.Broadcast();
}

void UFadeComponent::OnFadeOutFinished()
{
	OnFadeOutFinishedDelegate.Broadcast();
}
