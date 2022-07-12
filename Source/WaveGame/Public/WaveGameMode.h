// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

UENUM()
enum class WaveGameModeState : uint8
{
	PREWAVE = 0 UMETA(DisplayName = "PREWAVE"),
	WAVESPAWNING = 1  UMETA(DisplayName = "WAVESPAWNING"),
	WAVEINPROGRESS = 2     UMETA(DisplayName = "WAVEINPROGRESS"),
	WAVEENDED = 3     UMETA(DisplayName = "WAVEENDED"),
	GAMEOVER = 4     UMETA(DisplayName = "GAMEOVER")
};

/**
 * 
 */
UCLASS()
class WAVEGAME_API AWaveGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	float TimeBetweenWaves;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	TSubclassOf<class AEnemyAIBase> SpawnEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	TSubclassOf<class AEnemyAIBase> SpawnEnemyBoss;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	float SpawnCircleRadius;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	int32 MaxWaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	int32 EnemyWaveMultiplier;

protected:

	/*
		keeping a reference to it so we can cancel anytime we want
	*/
	FTimerHandle TimerHandle_EnemySpawner;

	/*
		holds number of enemies per wave. This changes in every wave
	*/
	int32 NumOfEnemiesToSpawn;

	int32 EnemyWaveCount;

	TEnumAsByte<WaveGameModeState> WaveStatus;

protected:

	UFUNCTION(Category = "GameMode")
	void SpawnNewEnemy();

	UFUNCTION()
	void StartWave();

	UFUNCTION()
	void EndWave();

	UFUNCTION()
	void PrepareForNextWave();

	UFUNCTION()
	void CheckWaveState();

public:

	AWaveGameMode();

	/*
		init call
	*/
	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
