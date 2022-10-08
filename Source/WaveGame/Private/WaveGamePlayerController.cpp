// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGamePlayerController.h"
#include "WaveGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "MasterView.h"

void AWaveGamePlayerController::BeginPlay()
{	
	UWaveGameInstance* CurrentGameInstance = Cast<UWaveGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (CurrentGameInstance)
	{
		CurrentGameInstance->InitiateUI();
	}
}
