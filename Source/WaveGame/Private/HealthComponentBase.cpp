// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponentBase.h"

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

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);


	FString LogMessage = GetOwner()->GetName() + " " + FString::SanitizeFloat(Health);
	// *FString because we need to convert that into character array
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *LogMessage);
}