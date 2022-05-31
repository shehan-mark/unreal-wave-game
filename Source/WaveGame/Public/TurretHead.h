// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurretHead.generated.h"

UCLASS()
class WAVEGAME_API ATurretHead : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TurretCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ABasicProjectile> ProjectileRef;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* BoxMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* CylinderMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MouseYaw(float Value);

	void MousePitch(float Value);

	FVector GetNormalizedVector(FVector Input);

	void CalculateTurretPitchVectors();

	FRotator GetVectorForTurretDirection();

	void Fire();

public:
	// Sets default values for this character's properties
	ATurretHead();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
