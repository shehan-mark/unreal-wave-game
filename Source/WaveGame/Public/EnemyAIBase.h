// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyAIBase.generated.h"

UENUM()
enum class EnemyState : uint8
{
	PREALIVE = 0 UMETA(DisplayName = "PREALIVE"),
	ALIVE = 1 UMETA(DisplayName = "ALIVE"),
	DEAD = 2 UMETA(DisplayName = "DEAD"),
	ATTACK = 3 UMETA(DisplayName = "ATTACK")
};

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

protected:

	TEnumAsByte<EnemyState> EnemyStatus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	// Sets default values for this pawn's properties
	AEnemyAIBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetEnemyStatus(TEnumAsByte<EnemyState> Status);

	TEnumAsByte<EnemyState> GetEnemyStatus();

};
