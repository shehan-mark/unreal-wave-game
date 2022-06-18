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
	APawn* Target;

	UPROPERTY(BlueprintReadOnly)
	bool bEnemyReachedPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	float TargetPointReachThreshold;

protected:

	UPROPERTY()
	class AEnemyAIBase* CurrentPawn;

	float GetEnemyToTargetPointLength();

	void UpdateEnemyLookRotation();

public:

	ABasicEnemyAIC();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void MovePawnToLocation(float DeltaSeconds);

	FVector GetNextPathPoint();

	UFUNCTION(BlueprintCallable)
	void CheckAndUpdateNextPathPoint();

	virtual void Tick(float DeltaTime) override;
};
