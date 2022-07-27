// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterView.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API UMasterView : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* MainViewWidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChildViewBase* StartMenu;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChildViewBase* PauseMenu;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChildViewBase* GameOverUI;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UChildViewBase* InGameHUD;

};
