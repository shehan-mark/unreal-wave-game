// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaveGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API AWaveGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMasterView> MasterViewRef;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> TestingWidget;

	class UMasterView* SpawnedMasterView;

protected:

	virtual void BeginPlay() override;
};
