// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaveGamePlayerController.generated.h"

/*
* Event delegates
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressEscape);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerReady);

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

	UPROPERTY()
	FOnPressEscape OnPressEscape;

	UPROPERTY()
	class UUserWidget* SpawnedWidget;

	UPROPERTY()
	class ATurretHead* OwningPawn;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode");
	TSubclassOf<class ATurretHead> SpawnPlayerTurret;

	UPROPERTY()
	FOnPlayerReady OnPlayerReady;

public:

	AWaveGamePlayerController();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void BroadcastEscape();

	virtual void OnPossess(APawn* aPawn) override;
};
