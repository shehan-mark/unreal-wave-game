// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasicEnemy.generated.h"

UCLASS()
class WAVEGAME_API ABasicEnemy : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	//https://docs.unrealengine.com/4.27/en-US/Basics/Components/Movement/
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move")
	class UCharacterMovementComponent* MovementComponent;*/

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeRef;

	UPROPERTY()
	class UBasicEnemyMovementComponent* EnemyMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this pawn's properties
	ABasicEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float InputValue);
};
