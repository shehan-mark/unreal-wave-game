// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

#include "WaveGamePlayerController.h"
#include "TurretHead.h"

void UInGameHUD::NativeConstruct()
{
	// Initial health status
	HealthBar_HUD->SetPercent(1);

	CurrentPlayerController = Cast<AWaveGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (CurrentPlayerController)
	{
		CurrentPlayerController->OnPlayerReady.AddDynamic(this, &UInGameHUD::BindPlayerEvents);
	}
}

void UInGameHUD::UpdateHealthBar(float Health)
{
	float PercentageValue = Health / 100.f;
	HealthBar_HUD->SetPercent(PercentageValue);
}

void UInGameHUD::BindPlayerEvents()
{
	CurrentPlayerController = Cast<AWaveGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (CurrentPlayerController && CurrentPlayerController->OwningPawn)
	{
		CurrentPlayerController->OwningPawn->OnHealthUpdate.AddDynamic(this, &UInGameHUD::UpdateHealthBar);
		UpdateHealthBar(100.f);
	}
}
