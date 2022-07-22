// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WaveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API UWaveGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	void InitiateHud();

	void StartGame();

	void ExitGame();

	void PauseGame();
};
