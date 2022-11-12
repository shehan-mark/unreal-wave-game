// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForcePush.generated.h"

UCLASS()
class WAVEGAME_API AForcePush : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForcePush();

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	class USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
