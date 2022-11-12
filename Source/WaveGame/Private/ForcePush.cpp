// Fill out your copyright notice in the Description page of Project Settings.


#include "ForcePush.h"

#include "Components/SphereComponent.h"

// Sets default values
AForcePush::AForcePush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	SphereComponent->InitSphereRadius(200.f);
	/*SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);*/
	SphereComponent->SetSimulatePhysics(false);
	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void AForcePush::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AForcePush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

