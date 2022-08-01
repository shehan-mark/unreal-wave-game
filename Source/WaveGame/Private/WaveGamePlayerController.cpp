// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGamePlayerController.h"
#include "WaveGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AWaveGamePlayerController::BeginPlay()
{
	UE_LOG(LogTemp, Error, TEXT("CALLING THIS -------------------------"));
	UWaveGameInstance* CurrentGameInstance = Cast<UWaveGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (CurrentGameInstance)
	{
		CurrentGameInstance->InitiateHud();
	}
}
