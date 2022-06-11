// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyAIBase.generated.h"

UCLASS()
class WAVEGAME_API AEnemyAIBase : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* FloatingPawnMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UHealthComponentBase* EnemyHealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeRef;

	bool ShouldStartMovement;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bUseVelocityChange;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float RequiredDistanceToTarget;

	UPROPERTY()
	FVector NextLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Sets default values for this pawn's properties
	AEnemyAIBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveEnemy();

	FVector GetNextPathPoint();
};
