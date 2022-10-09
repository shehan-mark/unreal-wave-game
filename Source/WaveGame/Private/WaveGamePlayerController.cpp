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
	
	EnableInput(this);

}

void AWaveGamePlayerController::Tick(float DeltaTime)
{
#if WITH_EDITOR
	if (IsInputKeyDown(EKeys::P))
	{
		// need to add a delay and broadcast the event. I dont want to broadcast this event multiple times
		OnPressEscape.Broadcast();
	}
#else
	if (IsInputKeyDown(EKeys::Escape))
	{
		OnPressEscape.Broadcast();
	}
#endif
}