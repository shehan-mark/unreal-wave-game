// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameInstance.h"
#include "MasterView.h"

void UWaveGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE IS RUNNING"));

}

void UWaveGameInstance::InitiateHud()
{
	UE_LOG(LogTemp, Error, TEXT("CALLING THIS -------------------------"));

	if (MasterViewRef != nullptr)
	{
		SpawnedMasterView = CreateWidget<UMasterView>(this, UMasterView::StaticClass());

		if (SpawnedMasterView)
		{
			UE_LOG(LogTemp, Error, TEXT("GAME INSTANCE SPAWNING VIEWPORT"));

			SpawnedMasterView->AddToViewport();
		}
	}
}
