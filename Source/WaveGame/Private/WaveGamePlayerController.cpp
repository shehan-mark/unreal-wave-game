// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveGamePlayerController.h"
#include "WaveGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "MasterView.h"
#include "TurretHead.h"

void AWaveGamePlayerController::AddPitchInput(float Val)
{
	RotationInput.Pitch += !IsLookInputIgnored() ? Val * InputPitchScale : 0.f;
	
	/*const float ScaledInput = Value * OwnPlayerController->InputYawScale;
	const float YawSum = LocalControlRotation.Yaw + ScaledInput * -1.f;
	if (YawSum > -90.f && YawSum < 90.f) OwnPlayerController->AddYawInput(Value);
	else if (LocalControlRotation.Yaw > -90.f && LocalControlRotation.Yaw < 90.f) {
		const float InputRemainder = FMath::Sign(Value) * FMath::Abs((90.f - FMath::Abs(LocalControlRotation.Yaw)) / OwnPlayerController->InputYawScale);
		OwnPlayerController->AddYawInput(InputRemainder);
	}*/

}

AWaveGamePlayerController::AWaveGamePlayerController()
{
}

void AWaveGamePlayerController::BeginPlay()
{
	UWaveGameInstance* CurrentGameInstance = Cast<UWaveGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (CurrentGameInstance)
	{
		CurrentGameInstance->InitiateUI();
	}
	ATurretHead* TurretPawn = Cast<ATurretHead>(this->GetPawn());
	if (TurretPawn)
	{
		OwningPawn = TurretPawn;
	}
	
	/*EnableInput(this);
	SetTickableWhenPaused(true);*/

}

void AWaveGamePlayerController::Tick(float DeltaTime)
{
}

void AWaveGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputActionBinding& PauseBinding = InputComponent->BindAction("Pause", IE_Pressed, this, &AWaveGamePlayerController::BroadcastEscape);
	PauseBinding.bConsumeInput = true;
	PauseBinding.bExecuteWhenPaused = true;
}

void AWaveGamePlayerController::BroadcastEscape()
{
	OnPressEscape.Broadcast();
}

void AWaveGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//UE_LOG(LogTemp, Error, TEXT("PLAYER CONTROLLER POSSES PAWN - %s"), *aPawn->GetName());
	ATurretHead* TurretPawn = Cast<ATurretHead>(aPawn);
	if (TurretPawn)
	{
		OwningPawn = TurretPawn;
		OnPlayerReady.Broadcast();
	}
}
