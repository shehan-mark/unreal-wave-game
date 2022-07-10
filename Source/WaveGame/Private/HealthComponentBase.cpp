// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponentBase.h"
#include "BasicProjectileDamage.h"
#include "EnemyAIBase.h"

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
		UE_LOG(LogTemp, Error, TEXT("UHealthComponentBase::HandleTakeAnyDamage CASTING SUCCESS"));
	}

	Health = FMath::Clamp(Health - CustomDamageType->ProjectileDamageAmount, 0.0f, DefaultHealth);

	if (Health == 0.0f)
	{
		//this->Destrory
		AController* APC = GetOwner()->GetInstigatorController();
		if (APC)
		{
			AEnemyAIBase* EnemyRef = Cast<AEnemyAIBase>(DamagedActor);
			if (EnemyRef)
			{
				EnemyRef->SetEnemyStatus(EnemyState::DEAD);
			}

			APC->UnPossess();
			GetOwner()->SetLifeSpan(3.0f);
		}
	}

	FString LogMessage = GetOwner()->GetName() + " " + FString::SanitizeFloat(Health);
	// *FString because we need to convert that into character array
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *LogMessage);
}