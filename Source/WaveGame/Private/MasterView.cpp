// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"

#include "MasterView.h"
#include "SubViewBase.h"
#include "WaveGamePlayerController.h"

void UMasterView::NativeConstruct()
{
	if (StartMenu_WBP)
	{
		StartMenu_WBP->ParentViewRef = this;
	}
	if (PauseMenu_WBP)
	{
		StartMenu_WBP->ParentViewRef = this;
	}

	OnStartGame.AddDynamic(this, &UMasterView::HandleStartGame);
	OnQuitGame.AddDynamic(this, &UMasterView::HandleQuitGame);

	CurrentMenuState = EMenuState::STARTMENU;
	CurrentPlayerController = Cast<AWaveGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (CurrentPlayerController)
	{
		CurrentPlayerController->OnPressEscape.AddDynamic(this, &UMasterView::HandleEscape);
	}
}

void UMasterView::HandleStartGame()
{
	CurrentMenuState = EMenuState::INROUND;
	WidgetSwitcherRoot->SetActiveWidgetIndex(1);
}

void UMasterView::HandleQuitGame()
{
	CurrentPlayerController->ConsoleCommand("quit");
}

void UMasterView::HandleEscape()
{
	bool isGamePaused = !CurrentPlayerController->IsPaused();
	//UE_LOG(LogTemp, Warning, TEXT("UMasterView::HandleEscape - Calling here %s ----"), (isGamePaused == true ? TEXT("True"): TEXT("False")));

	if (isGamePaused)
	{
		CurrentMenuState = EMenuState::PAUSED;
		WidgetSwitcherRoot->SetActiveWidgetIndex(2);
	}
	else
	{
		CurrentMenuState = EMenuState::INROUND;
		WidgetSwitcherRoot->SetActiveWidgetIndex(1);
	}
	CurrentPlayerController->SetPause(isGamePaused);
}

void UMasterView::HandleResume()
{
	CurrentMenuState = EMenuState::INROUND;
	CurrentPlayerController->SetPause(false);
	WidgetSwitcherRoot->SetActiveWidgetIndex(1);
}

void UMasterView::HandleBackToMainMenu()
{
	CurrentMenuState = EMenuState::STARTMENU;
	WidgetSwitcherRoot->SetActiveWidgetIndex(0);
}
