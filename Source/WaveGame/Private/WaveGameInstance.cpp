// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameInstance.h"

#include "MasterView.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UWaveGameInstance::UWaveGameInstance(const FObjectInitializer& ObjectIntializer)
{
	if (MasterViewRef == nullptr) return;
}

void UWaveGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE INIT..."));

}

void UWaveGameInstance::InitiateUI()
{
	if (MasterViewRef != nullptr)
	{
		UMasterView* WidgetCreated = CreateWidget<UMasterView>(this, UMasterView::StaticClass());
		WidgetCreated->AddToViewport();

		APlayerController* PlayerController = GetFirstLocalPlayerController();

		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(WidgetCreated->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);

		PlayerController->bShowMouseCursor = true;
	}
}
