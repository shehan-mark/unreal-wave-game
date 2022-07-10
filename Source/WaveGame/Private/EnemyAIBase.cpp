// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "HealthComponentBase.h"

#include "NavigationSystem.h" 
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemyAIBase::AEnemyAIBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	SphereComponent->InitSphereRadius(20.f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetSimulatePhysics(true);
	RootComponent = SphereComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetupAttachment(SphereComponent);

	EnemyHealthComponent = CreateDefaultSubobject<UHealthComponentBase>(TEXT("EnemyHealthComponent"));

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovementComponent"));
	FloatingPawnMovementComponent->MaxSpeed = 600.f;
	EnemyStatus = EnemyState::PREALIVE;
}

// Called when the game starts or when spawned
void AEnemyAIBase::BeginPlay()
{
	Super::BeginPlay();
	EnemyStatus = EnemyState::ALIVE;
}

// Called every frame
void AEnemyAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAIBase::SetEnemyStatus(TEnumAsByte<EnemyState> Status)
{
	EnemyStatus = Status;
}

TEnumAsByte<EnemyState> AEnemyAIBase::GetEnemyStatus()
{
	return EnemyStatus;
}
