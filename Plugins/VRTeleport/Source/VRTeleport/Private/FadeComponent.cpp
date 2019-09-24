#include "FadeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"

UFadeComponent::UFadeComponent()
{
	bWantsBeginPlay = true;

	FadeInSettings.ToAlpha = 1.f;
	FadeInSettings.Duration = 0.3f;

	FadeOutSettings.FromAlpha = 1.f;
	FadeOutSettings.ToAlpha = 0.f;
	FadeOutSettings.Duration = 0.3f;
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
	Fade(FadeInSettings);
	OnFadeInStartedDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(FadeInFinishedTimer, this, &UFadeComponent::OnFadeInFinished, FadeInSettings.Duration);
}

void UFadeComponent::FadeOut()
{
	Fade(FadeOutSettings);
	OnFadeOutStartedDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(FadeInFinishedTimer, this, &UFadeComponent::OnFadeOutFinished, FadeOutSettings.Duration);
}

bool UFadeComponent::IsPlaying()
{
	return IsTimerPlaying(FullFade) || IsTimerPlaying(FadeInFinishedTimer) || IsTimerPlaying(FadeOutFinishedTimer);
}

bool UFadeComponent::IsTimerPlaying(const FTimerHandle& TimerHandle)
{
	return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle);
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
	//OnFadeInFinishedDelegate.Broadcast();
}

void UFadeComponent::OnFadeOutFinished()
{
	OnFadeOutFinishedDelegate.Broadcast();
}
