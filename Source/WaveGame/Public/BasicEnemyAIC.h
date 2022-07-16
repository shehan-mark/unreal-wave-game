// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicEnemyAIC.generated.h"

/**
 * 
 */
UCLASS()
class WAVEGAME_API ABasicEnemyAIC : public AAIController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Miscellaneous")
	bool bLoggsEnabled;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MovementStoppingRadius;

	UPROPERTY()
	FVector NextPathPoint;

	UPROPERTY()
	bool bShouldStopMovement;

	UPROPERTY()
	APawn* Target;

	/*
		we use this value in BTT stop or exit from Enemy Move 
	*/
	UPROPERTY(BlueprintReadOnly)
	bool bEnemyReachedPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float TargetPointReachThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float EnemyMovementSpeed;

protected:

	UPROPERTY()
	class AEnemyAIBase* CurrentPawn;

	/*
		enemy current objective
	*/
	FVector CurrentGoalLocation;

protected:

	float GetEnemyToTargetPointLength(FVector TargetPoint);

	void UpdateEnemyLookRotation();

public:

	ABasicEnemyAIC();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void MovePawnToLocation(float DeltaSeconds);

	FVector GetNextPathPoint();

	UFUNCTION(BlueprintCallable)
	void CheckAndUpdateNextPathPoint();

	UFUNCTION(BlueprintCallable)
	void AttackTarget();

	virtual void Tick(float DeltaTime) override;
};
