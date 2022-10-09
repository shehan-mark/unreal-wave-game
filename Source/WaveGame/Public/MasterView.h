// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterView.generated.h"


/*
* Status of Menu
*/
UENUM()
enum class EMenuState : uint8
{
	STARTMENU = 0 UMETA(DisplayName = "STARTMENU"),
	INROUND = 1  UMETA(DisplayName = "INROUND"),
	PAUSED = 2     UMETA(DisplayName = "PAUSED"),
	GAMEOVER = 3     UMETA(DisplayName = "GAMEOVER"),
};


/*
* Event delegates
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenu);

/**
 * Master UI Widget that holds all other widgets during the game
 */
UCLASS()
class WAVEGAME_API UMasterView : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcherRoot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USubViewBase* StartMenu_WBP;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USubViewBase* InGameHUD_WBP;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USubViewBase* PauseMenu_WBP;

	UPROPERTY()
	FOnStartGame OnStartGame;

	UPROPERTY()
	FOnQuitGame OnQuitGame;

	UPROPERTY()
	FOnResumeGame OnResumeGame;

	UPROPERTY()
	FOnMainMenu OnMainMenu;

	EMenuState CurrentMenuState;

	class AWaveGamePlayerController* CurrentPlayerController;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleStartGame();

	UFUNCTION()
	void HandleQuitGame();

	UFUNCTION()
	void HandleEscape();

	UFUNCTION()
	void HandleResume();
	
	UFUNCTION()
	void HandleBackToMainMenu();
};
