// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveGameInstance.generated.h"

/**
 * https://www.youtube.com/watch?v=XKNIQga3HYk - init UI
 */
UCLASS()
class WAVEGAME_API UWaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UWaveGameInstance(const FObjectInitializer& ObjectIntializer);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMasterView> MasterViewRef;
	
	class UMasterView* SpawnedMasterView;

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void InitiateUI();

	void StartGame();

	void ExitGame();

	void PauseGame();
};
