// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurretHead.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdate, float, HealthNow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);

UENUM(BlueprintType)
enum class TurretState : uint8
{
	ALIVE = 0 UMETA(DisplayName = "ALIVE"),
	DEAD = 1 UMETA(DisplayName = "DEAD")
};

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

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Detector")
	class UBoxComponent* EnemyNotifierBox;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float LifeSpanAfterDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UHealthComponentBase* HealthComponent;

	UPROPERTY()
	FOnHealthUpdate OnHealthUpdate;

	UPROPERTY()
	FOnPlayerDied OnPlayerDied;

protected:

	TurretState TurretStatus;

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

	void Die();

	TurretState GetTurretStatus();

	void SetTurretStatus(TurretState State);

	void Reset();
};
