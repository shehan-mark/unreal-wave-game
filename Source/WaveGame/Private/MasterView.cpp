// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterView.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h" 

#include "WaveGameMode.h"
#include "SubViewBase.h"
#include "WaveGamePlayerController.h"
#include "TurretHead.h"
#include "GameOverView.h"

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
		CurrentPlayerController->OnPlayerReady.AddDynamic(this, &UMasterView::BindPlayerEvents);
	}

	PlayerScore = 0.0f;
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
	if (CurrentPlayerController->IsPaused())
	{
		CurrentPlayerController->SetPause(false);
	}
	AWaveGameMode* GameMode = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->DestroyAndStartOver();
		// reseting player score
		PlayerScore = 0.0f;
		CurrentPlayerController->OwningPawn->ResetPlayerScore();
	}
	UpdateUIToState();
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
	UGameOverView* GameOverView = Cast<UGameOverView>(GameOver_WBP);
	if (GameOverView)
	{
		FText TextScore = FText::FromString(FString::SanitizeFloat(PlayerScore));
		GameOverView->ScoreTextBlock_Value->SetText(TextScore);
	}
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

void UMasterView::BindPlayerEvents()
{
	if (CurrentPlayerController && IsValid(CurrentPlayerController->OwningPawn))
	{
		CurrentPlayerController->OwningPawn->OnPlayerDied.AddDynamic(this, &UMasterView::GameOver);
		CurrentPlayerController->OwningPawn->OnPlayerScored.AddDynamic(this, &UMasterView::GetPlayerScore);
	}
}

void UMasterView::HandleQuitGame()
{
	CurrentPlayerController->ConsoleCommand("quit");
}

void UMasterView::GetPlayerScore(bool Reset)
{
	if (CurrentPlayerController && IsValid(CurrentPlayerController->OwningPawn))
	{
		PlayerScore = CurrentPlayerController->OwningPawn->GetScore();
		UE_LOG(LogTemp, Error, TEXT("PLAYER SCORE %f"), PlayerScore);
	}
}