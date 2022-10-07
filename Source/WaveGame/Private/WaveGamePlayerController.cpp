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
		UE_LOG(LogTemp, Warning, TEXT("AWaveGamePlayerController::BeginPlay - GameInstance Check"));
		CurrentGameInstance->InitiateUI();

	}
	
	
	
	/*UE_LOG(LogTemp, Error, TEXT("CALLING THIS -------------------------"));
	if (CurrentGameInstance)
	{
		CurrentGameInstance->InitiateHud();
	}*/

	/*if (MasterViewRef != nullptr)
	{
		SpawnedMasterView = CreateWidget<UMasterView>(CurrentGameInstance, UMasterView::StaticClass());

		if (SpawnedMasterView)
		{
			UE_LOG(LogTemp, Error, TEXT("ADDING MASTER VIEW TO VIEWPORT"));

			SpawnedMasterView->AddToViewport();
		}
	}*/

	/*if (TestingWidget != nullptr)
	{
		UUserWidget* SpawningThing = CreateWidget<UUserWidget>(this, UUserWidget::StaticClass());
		if (SpawningThing)
		{
			SpawningThing->AddToViewport();
			UE_LOG(LogTemp, Error, TEXT("ADDING VIEW TO VIEWPORT"));
		}
	}*/
}
