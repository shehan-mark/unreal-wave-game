// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyAIBase.generated.h"

UENUM(BlueprintType)
enum class EnemyState : uint8
{
	IDLE = 0 UMETA(DisplayName = "IDLE"),
	MOVING = 1 UMETA(DisplayName = "MOVING"),
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UHealthComponentBase* EnemyHealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	class ATurretHead* CurrentDamageTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	float LifeSpanAfterDeath;

protected:

	EnemyState EnemyStatus;

	UPROPERTY()
	bool Stunned;

	FTimerHandle TimerHandle_UnStun;

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

	void SetEnemyStatus(EnemyState Status);

	UFUNCTION(BlueprintCallable)
	EnemyState GetEnemyStatus();

	void DoDamage();

	void Die();

	void Stun(float StunDuration);

	void UnStun();

	bool IsStunned();

};
