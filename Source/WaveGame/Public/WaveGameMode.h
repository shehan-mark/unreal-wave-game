// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaveGameMode.generated.h"

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

protected:

	UFUNCTION(Category = "GameMode")
	void SpawnNewEnemy();

	UFUNCTION()
	void StartWave();

	UFUNCTION()
	void EndWave();

	UFUNCTION()
	void PrepareForNextWave();

public:

	AWaveGameMode();

	/*
		init call
	*/
	virtual void StartPlay() override;

};
