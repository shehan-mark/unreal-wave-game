// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGameInstance.h"

#include "MasterView.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UWaveGameInstance::UWaveGameInstance(const FObjectInitializer& ObjectIntializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MasterViewWidget(TEXT("/Game/Blueprints/UI/TESTWIDGET"));
	if (!ensure(MasterViewWidget.Class != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("UWaveGameInstance::InitiateUI - Cannot Find UI"));
		return;
	}

	MasterViewRef = MasterViewWidget.Class;
}

void UWaveGameInstance::InitiateUI()
{
	UE_LOG(LogTemp, Warning, TEXT("UWaveGameInstance::InitiateUI - GameInstance InitiateUI Check"));

	
	UE_LOG(LogTemp, Error, TEXT("UWaveGameInstance::InitiateUI - UI Check %s"), *MasterViewRef->GetName());

	SpawnedMasterView = CreateWidget<UUserWidget>(this, MasterViewRef);
	SpawnedMasterView->AddToViewport();

	/* Get player controller reference */
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	/* Setup input parameters to the SetInputMode function */
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(SpawnedMasterView->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	/* Set input mode */
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}
