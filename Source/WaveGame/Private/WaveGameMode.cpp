// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameMode.h"
#include "TimerManager.h"
#include "EnemyAIBase.h"

AWaveGameMode::AWaveGameMode()
{
	TimeBetweenWaves = 2.0f;
	SpawnCircleRadius = 1000.0f;
	MaxWaveCount = 5;
	EnemyWaveMultiplier = 2;

	PrimaryActorTick.TickInterval = 1.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void AWaveGameMode::StartGame()
{
	PrepareForNextWave();
}

void AWaveGameMode::StartPlay()
{
	Super::StartPlay();

	//PrepareForNextWave();
}

void AWaveGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void AWaveGameMode::PrepareForNextWave()
{
	WaveStatus = WaveGameModeState::PREWAVE;

	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AWaveGameMode::StartWave, TimeBetweenWaves, false);	
}

void AWaveGameMode::CheckWaveState()
{
	/*
		only run check logic when a wave is in progress
	*/
	if (WaveStatus != WaveGameModeState::WAVEINPROGRESS)
		return;

	bool bIsAnyEnemyAlive = false;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* _Pawn = It->Get();
		AEnemyAIBase* Enemy = Cast<AEnemyAIBase>(_Pawn);
		if (Enemy && Enemy->GetEnemyStatus() != EnemyState::DEAD)
		{
			bIsAnyEnemyAlive = true;
			break;
		}
	}

	if (!bIsAnyEnemyAlive)
	{
		EndWave();
	}
	
}

void AWaveGameMode::SpawnNewEnemy()
{
	if (IsValid(SpawnEnemy))
	{
		// spawn logic should go here
		FActorSpawnParameters SpawnParams;

		// finding spawn location in the circle
		float Theta = FMath::DegreesToRadians(FMath::RandRange(0.0f, 359.0f));
		//UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - Theta %f"), Theta);
		float XCoordinate = FMath::Cos(Theta) * SpawnCircleRadius;
		float YCoordinate = FMath::Sin(Theta) * SpawnCircleRadius;
		/*UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - XCoordinate %f"), XCoordinate);
		UE_LOG(LogTemp, Warning, TEXT("AWaveGameMode::SpawnNewEnemy - YCoordinate %f"), YCoordinate);*/

		AEnemyAIBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyAIBase>(SpawnEnemy, FVector(XCoordinate, YCoordinate, 20.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);

		// maintain of enemy loop game logic
		NumOfEnemiesToSpawn--;

		if (NumOfEnemiesToSpawn <= 0)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_EnemySpawner);
			WaveStatus = WaveGameModeState::WAVEINPROGRESS;
			//EndWave();
		}
	}
}

void AWaveGameMode::StartWave()
{
	//UE_LOG(LogTemp, Error, TEXT("AWaveGameMode::StartWave - Starting Now..."));

	EnemyWaveCount++;

	NumOfEnemiesToSpawn = EnemyWaveMultiplier * EnemyWaveCount;

	WaveStatus = WaveGameModeState::WAVESPAWNING;
	GetWorldTimerManager().SetTimer(TimerHandle_EnemySpawner, this, &AWaveGameMode::SpawnNewEnemy, 1.0f, true, 0.0f);
}

void AWaveGameMode::EndWave()
{
	WaveStatus = WaveGameModeState::WAVEENDED;

	if (EnemyWaveCount >= MaxWaveCount - 1)
	{
		WaveStatus = WaveGameModeState::GAMEOVER;
		//UE_LOG(LogTemp, Error, TEXT("AWaveGameMode::EndWave - GAME OVER..."));
	}
	else
	{
		PrepareForNextWave();
	}
}


