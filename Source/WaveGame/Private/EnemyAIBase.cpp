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

	ShouldStartMovement = false;
	MovementForce = 1000.0f;
	bUseVelocityChange = true;
	RequiredDistanceToTarget = 100.f;
}

// Called when the game starts or when spawned
void AEnemyAIBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShouldStartMovement)
	{
		MoveEnemy();
	}
}

void AEnemyAIBase::MoveEnemy()
{

	////FVector NextLocation = GetNextPathPoint();
	//UE_LOG(LogTemp, Warning, TEXT("Location NEXT %f, %f, %f"), NextLocation.X, NextLocation.Y, NextLocation.Z);
	////DrawDebugSphere(GetWorld(), NextLocation, 10, 26, FColor::Red, true, -1, 0, 2);
	//FVector CurrentLocation = GetActorLocation();

	//float DistanceToTarget = (CurrentLocation - NextLocation).Size();

	//if (DistanceToTarget <= RequiredDistanceToTarget)
	//{
	//	NextLocation = GetNextPathPoint();
	//}
	//else
	//{
	//	FVector ForceDirection = NextLocation - CurrentLocation;
	//	ForceDirection.Normalize();
	//	ForceDirection *= MovementForce;

	//	DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
	//	SphereComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	//	//MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	//}
}

FVector AEnemyAIBase::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	FVector CurrentLocaiton = GetActorLocation();

	// I had to add Character.h header file in order for below function to understand that ACharacter is a type of Pawn.
	//UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, CurrentLocaiton, PlayerPawn);
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, CurrentLocaiton, FVector(0.0f, 0.0f, 0.0f));

	if (NavigationPath->PathPoints.Num() > 1)
	{
		// getting index 1 because 0 is always the current location.
		return NavigationPath->PathPoints[1];
	}
	return CurrentLocaiton;
}
