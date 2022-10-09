// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubViewBase.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API UInGameHUD : public USubViewBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar_HUD;

public:

	void SetPlayerAndSetActive();

	void UpdateHealthBar();

};
