// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"

#include "WaveGameMode.h"
#include "MasterView.h"
#include "SubViewBase.h"
#include "WaveGamePlayerController.h"

void UMasterView::NativeConstruct()
{
	bIsFocusable = true;
	if (StartMenu_WBP)
	{
		StartMenu_WBP->ParentViewRef = this;
	}
	if (PauseMenu_WBP)
	{
		PauseMenu_WBP->ParentViewRef = this;
	}
	if (GameOver_WBP)
	{
		GameOver_WBP->ParentViewRef = this;
	}
	if (InGameHUD_WBP)
	{
		InGameHUD_WBP->ParentViewRef = this;
	}

	OnStartGame.AddDynamic(this, &UMasterView::StartGame);
	OnMainMenu.AddDynamic(this, &UMasterView::ViewMainMenu);
	OnResumeGame.AddDynamic(this, &UMasterView::ResumeGame);
	OnQuitGame.AddDynamic(this, &UMasterView::HandleQuitGame);

	CurrentPlayerController = Cast<AWaveGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (CurrentPlayerController)
	{
		CurrentPlayerController->OnPressEscape.AddDynamic(this, &UMasterView::HandleEscape);
	}

	// intial menu
	ViewMainMenu();
}

void UMasterView::UpdateUIToState()
{
	switch (CurrentMenuState)
	{
	case EMenuState::STARTMENU:
		CurrentActiveWidget = StartMenu_WBP;
		WidgetSwitcherRoot->SetActiveWidgetIndex(0);
		EnableUserInteractionsForUI();
		break;
	case EMenuState::INROUND:
		CurrentActiveWidget = InGameHUD_WBP;
		WidgetSwitcherRoot->SetActiveWidgetIndex(1);
		DisableUserInteractionsForUI();
		break;
	case EMenuState::PAUSED:
		CurrentActiveWidget = PauseMenu_WBP;
		WidgetSwitcherRoot->SetActiveWidgetIndex(2);
		EnableUserInteractionsForUI();
		break;
	case EMenuState::GAMEOVER:
		CurrentActiveWidget = GameOver_WBP;
		WidgetSwitcherRoot->SetActiveWidgetIndex(3);
		EnableUserInteractionsForUI();
		break;
	}
}

void UMasterView::ViewMainMenu()
{
	CurrentMenuState = EMenuState::STARTMENU;
	UpdateUIToState();
}

void UMasterView::StartGame()
{

	CurrentMenuState = EMenuState::INROUND;
	UpdateUIToState();
	if (CurrentPlayerController->IsPaused())
	{
		CurrentPlayerController->SetPause(false);
	}
	AWaveGameMode* GameMode = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->DestroyAndStartOver();
	}
}

void UMasterView::PauseGame()
{
	CurrentMenuState = EMenuState::PAUSED;
	CurrentPlayerController->SetPause(true);
	UpdateUIToState();
}

void UMasterView::ResumeGame()
{
	CurrentMenuState = EMenuState::INROUND;
	CurrentPlayerController->SetPause(false);
	UpdateUIToState();
}

void UMasterView::GameOver()
{
	CurrentMenuState = EMenuState::GAMEOVER;
	UpdateUIToState();
}

void UMasterView::HandleEscape()
{
	bool isGamePaused = CurrentPlayerController->IsPaused();
	if (isGamePaused)
	{
		bool _SuccessUnPausing = CurrentPlayerController->SetPause(false);
		if (_SuccessUnPausing)
		{
			CurrentMenuState = EMenuState::INROUND;
			UpdateUIToState();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FAILED TO UNPAUSE"));
		}
	}
	else
	{
		bool _SuccessPausing = CurrentPlayerController->SetPause(true);
		if (_SuccessPausing)
		{
			PauseGame();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FAILED TO PAUSE"));
		}
	}
}


void UMasterView::EnableUserInteractionsForUI()
{
	/* Setup input parameters to the SetInputMode function */
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);


	/* Set input mode */
	CurrentPlayerController->SetInputMode(InputModeData);
	CurrentPlayerController->bShowMouseCursor = true;
}

void UMasterView::DisableUserInteractionsForUI()
{
	FInputModeGameOnly InputModeData;
	CurrentPlayerController->SetInputMode(InputModeData);
	CurrentPlayerController->bShowMouseCursor = false;
}

void UMasterView::HandleQuitGame()
{
	CurrentPlayerController->ConsoleCommand("quit");
}