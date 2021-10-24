// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* init CameraBoom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // distance that the camera sits behind the player
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerFollowCamera"));
	PlayerFollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attaching camera to end of the boom by specifying boom end socket name
	PlayerFollowCamera->bUsePawnControlRotation = false; // camera only rotates relative to the boom


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation(); // getting forward rotation/ vector relative to world direction
		UE_LOG(LogTemp, Warning, TEXT("Current Rotation %f - %f - %f"), Rotation.Vector().X, Rotation.Vector().Y, Rotation.Vector().Z);

		const FRotator NewYawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		//AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation(); // getting forward rotation/ vector relative to world direction
		UE_LOG(LogTemp, Warning, TEXT("Current Rotation %f - %f - %f"), Rotation.Vector().X, Rotation.Vector().Y, Rotation.Vector().Z);

		const FRotator NewYawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		//AddMovementInput(GetActorForwardVector(), Value);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

