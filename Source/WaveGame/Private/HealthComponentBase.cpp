// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponentBase.h"
#include "BasicProjectileDamage.h"
#include "Kismet/GameplayStatics.h"

#include "TurretHead.h"
#include "EnemyAIBase.h"
#include "WaveGamePlayerController.h"

// Sets default values for this component's properties
UHealthComponentBase::UHealthComponentBase()
{

	DefaultHealth = 100;
	Health = DefaultHealth;
}


// Called when the game starts
void UHealthComponentBase::BeginPlay()
{
	Super::BeginPlay();

	/*
		all components have their own owners
		subscribing to owner's damage event
	*/
	AActor* OwnOwner = GetOwner();
	if (OwnOwner)
	{
		OwnOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponentBase::HandleTakeAnyDamage);
	}
	
}

void UHealthComponentBase::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	/*
	*	issue I got by normal casting
		https://stackoverflow.com/questions/48759558/a-value-of-type-const-char-cannot-be-used-to-initialize-an-entity-of-type-ch
		issue resolved by casting like below. Dont know the difference
		https://forums.unrealengine.com/t/c-casting-acharacter-child-pointer-to-aactor-does-not-work/405575
	*/
	UBasicProjectileDamage* CustomDamageType = (UBasicProjectileDamage*)DamageType;
	if (CustomDamageType)
	{
		//UE_LOG(LogTemp, Error, TEXT("UHealthComponentBase::HandleTakeAnyDamage CASTING SUCCESS"));
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	ATurretHead* PlayerRef = Cast<ATurretHead>(DamagedActor);
	if (PlayerRef)
	{
		PlayerRef->OnHealthUpdate.Broadcast(Health);
	}
	if (Health == 0.0f)
	{
		AEnemyAIBase* EnemyRef = Cast<AEnemyAIBase>(DamagedActor);
		if (EnemyRef)
		{
			//UE_LOG(LogTemp, Error, TEXT("DAMAGE INSTIGATOR %s"), *InstigatedBy->GetName());
			LetPlayerKnowEnemyKill(InstigatedBy);
			EnemyRef->Die();
		}

		if (PlayerRef)
		{
			PlayerRef->Die();
		}

	}

	//FString LogMessage = GetOwner()->GetName() + " " + FString::SanitizeFloat(Health);
	// *FString because we need to convert that into character array
	//UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *LogMessage);
}

void UHealthComponentBase::ResetHealth()
{
	Health = DefaultHealth;
	ATurretHead* PlayerRef = Cast<ATurretHead>(GetOwner());
	if (PlayerRef)
	{
		PlayerRef->OnHealthUpdate.Broadcast(Health);
	}
}

void UHealthComponentBase::LetPlayerKnowEnemyKill(AController* InstigatedBy)
{
	AWaveGamePlayerController* CurrentPlayerController = Cast<AWaveGamePlayerController>(InstigatedBy);
	if (CurrentPlayerController)
	{
		CurrentPlayerController->OwningPawn->OnPlayerScored.Broadcast(false);
	}

}
