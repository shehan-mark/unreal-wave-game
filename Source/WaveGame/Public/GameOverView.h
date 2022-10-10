// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubViewBase.h"
#include "GameOverView.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API UGameOverView : public USubViewBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ScoreTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* MainMenuButton;

protected:

	virtual void NativeConstruct() override;

	void OnMainMenuPressed(); 
};
