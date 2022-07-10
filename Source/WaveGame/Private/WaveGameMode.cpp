// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "TimerManager.h"
#include "EnemyAIBase.h"

AWaveGameMode::AWaveGameMode()
{
	TimeBetweenWaves = 2.0f;
	SpawnCircleRadius = 1000.0f;
}

void AWaveGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void AWaveGameMode::PrepareForNextWave()
{
	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AWaveGameMode::StartWave, TimeBetweenWaves, false);	
}

void AWaveGameMode::SpawnNewEnemy()
{
	if (IsValid(SpawnEnemy))
	{
		// spawn logic should go here
		FActorSpawnParameters SpawnParams;

		// finding spawn location in the circle
		float Theta = FMath::DegreesToRadians(FMath::RandRange(0.0f, 359.0f));
		UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - Theta %f"), Theta);
		float XCoordinate = FMath::Cos(Theta) * SpawnCircleRadius;
		float YCoordinate = FMath::Sin(Theta) * SpawnCircleRadius;
		UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - XCoordinate %f"), XCoordinate);
		UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - YCoordinate %f"), YCoordinate);

		AEnemyAIBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyAIBase>(SpawnEnemy, FVector(XCoordinate, YCoordinate, 20.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

		// maintain of enemy loop game logic
		NumOfEnemiesToSpawn--;

		if (NumOfEnemiesToSpawn <= 0)
		{
			EndWave();
		}
	}
}

void AWaveGameMode::StartWave()
{
	UE_LOG(LogTemp, Error, TEXT("AWaveGameMode::StartWave - Starting Now..."));

	EnemyWaveCount++;

	NumOfEnemiesToSpawn = 2 * EnemyWaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_EnemySpawner, this, &AWaveGameMode::SpawnNewEnemy, 1.0f, true, 0.0f);
}

void AWaveGameMode::EndWave()
{
	UE_LOG(LogTemp, Error, TEXT("AWaveGameMode::EndWave - Ending Now..."));

	GetWorldTimerManager().ClearTimer(TimerHandle_EnemySpawner);
	PrepareForNextWave();
}


