// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterView.generated.h"


/*
* Event delegates
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitGame);

/**
 * 
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

	FOnStartGame OnStartGame;

	FOnQuitGame OnQuitGame;

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleStartGame();

	UFUNCTION()
	void HandleQuitGame();
};
