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
	CameraBoom->TargetArmLength = 450.0f; // distance that the camera sits behind the player
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerFollowCamera"));
	PlayerFollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attaching camera to end of the boom by specifying boom end socket name
	PlayerFollowCamera->bUsePawnControlRotation = false; // camera only rotates relative to the boom

	/*
		these are by default unset/ false
		but what these do is disabling object rotation according to the control rotation. Enable these for better understanding
	*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 450.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	//GetCharacterMovement()->AirControl = 0.2f;
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

/*
	some reference regarding rotation - https://unrealcpp.com/rotating-actor/
	rotation matrix in math - https://www.youtube.com/watch?v=OYuoPTRVzxY
*/
void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation(); // getting forward rotation/ vector relative to world direction
		//UE_LOG(LogTemp, Warning, TEXT("Current Rotation %f - %f - %f"), Rotation.Vector().X, Rotation.Vector().Y, Rotation.Vector().Z);

		FRotator NewYawRotation(0, Rotation.Yaw, 0); // this confuses me
		//FRotator NewYawRotation = FRotator(0, Rotation.Yaw, 0);
		// FRotationMatrix(NewYawRotation) - Giving a rotation and taking back a vector. Because we need a vector to decide the direction that we need to go
		// .GetUnitAxis(EAxis::X) - This means give me the direction difference from the global X axis. Because our forward is X and it is normalized
		FVector Direction = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::X);
		UE_LOG(LogTemp, Warning, TEXT("Current Direction %f - %f - %f"), Direction.X, Direction.Y, Direction.Z);

		AddMovementInput(Direction, Value);

		//AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation(); // getting forward rotation/ vector relative to world direction
		//UE_LOG(LogTemp, Warning, TEXT("Current Rotation %f - %f - %f"), Rotation.Vector().X, Rotation.Vector().Y, Rotation.Vector().Z);

		FRotator NewYawRotation = FRotator(0, Rotation.Yaw, 0);
		FVector Direction = FRotationMatrix(NewYawRotation).GetUnitAxis(EAxis::Y);
		UE_LOG(LogTemp, Warning, TEXT("Current Direction %f - %f - %f"), Direction.X, Direction.Y, Direction.Z);

		AddMovementInput(Direction, Value);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

