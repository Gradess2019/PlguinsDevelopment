#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "TeleportStructs.h"
#include "FadeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeInStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeInFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeOutStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFadeOutFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTELEPORT_API UFadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFadeComponent();

	UPROPERTY(BlueprintAssignable, Category = "FadeComponent")
	FFadeInStarted OnFadeInStartedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "FadeComponent")
	FFadeInFinished OnFadeInFinishedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "FadeComponent")
	FFadeOutStarted OnFadeOutStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "FadeComponent")
	FFadeOutFinished OnFadeOutFinishedDelegate;
	
	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "FadeComponent")
	void StartFade();

	UFUNCTION(BlueprintCallable, Category = "FadeComponent")
	void StopFade();
	
	UFUNCTION(BlueprintCallable, Category = "FadeComponent")
	void FadeIn();
	
	UFUNCTION(BlueprintCallable, Category = "FadeComponent")
	void FadeOut();

	UFUNCTION(BlueprintCallable, Category = "FadeComponent")
	bool IsPlaying();


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FadeComponent")
	int PlayerID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FadeComponent")
	FFadeSettings FadeInSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FadeComponent")
	FFadeSettings FadeOutSettings;

	UPROPERTY(BlueprintReadOnly, Category = "FadeComponent")
	TWeakObjectPtr<APlayerCameraManager> CameraManager;

private:

	FTimerHandle FullFade;
	FTimerHandle FadeInFinishedTimer;
	FTimerHandle FadeOutFinishedTimer;

	UFUNCTION()
	void Fade(const FFadeSettings& FADE_SETTINGS);
	
	UFUNCTION()
	void OnFadeInFinished();

	UFUNCTION()
	void OnFadeOutFinished();

	void ClearTimers();

	bool IsTimerPlaying(const FTimerHandle& TimerHandle);
};

