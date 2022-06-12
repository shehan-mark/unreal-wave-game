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

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardComponent* BlackBoardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MovementStoppingRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bShouldStopMoving;

	UPROPERTY()
	FVector NextPathPoint;

	UPROPERTY()
	APawn* Target;

protected:

	UPROPERTY()
	class AEnemyAIBase* CurrentPawn;

public:

	ABasicEnemyAIC();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void MovePawnToLocation(FVector Location = FVector(0.0f, 0.0f, 0.0f));

	FVector GetNextPathPoint();

	UFUNCTION(BlueprintCallable)
	void MoveAlongThePath();

};
