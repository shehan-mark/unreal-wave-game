// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BasicEnemyMovementComponent.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);
	MeshComponent->SetCanEverAffectNavigation(false);

	/*MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = SphereComponent;*/

	// reason I didnt call this in BeginPlay
	// https://forums.unrealengine.com/t/when-is-valid-time-to-create-subobjects-components/367184
	//BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	/*if (BlackBoardComponent != nullptr && BehaviorTreeRef != nullptr)
	{
		BlackBoardComponent->InitializeBlackboard(*BehaviorTreeRef->BlackboardAsset);
	}*/

	// Create an instance of our movement component, and tell it to update the root.
	EnemyMovementComponent = CreateDefaultSubobject<UBasicEnemyMovementComponent>(TEXT("EnemyMovementComponent"));
	EnemyMovementComponent->UpdatedComponent = RootComponent;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Blueprints/Enemy/Spider_BT.Spider_BT'"));
	if (obj.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemy::ABasicEnemy Object finder succeeded"));
		BehaviorTreeRef = obj.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABasicEnemy::ABasicEnemy object finder failed"));
	}

	// Take control of the default player
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*AController* WhichController = GetController();
	AAIController* AIC = Cast<AAIController>(WhichController);
	if (AIC != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ABasicEnemy::BeginPlay ===== Controller is AI"));
	}*/
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasicEnemy::MoveForward);
}

UPawnMovementComponent* ABasicEnemy::GetMovementComponent() const
{
	return EnemyMovementComponent;
}

void ABasicEnemy::MoveForward(float InputValue)
{
	if (EnemyMovementComponent && (EnemyMovementComponent->UpdatedComponent == RootComponent))
	{
		EnemyMovementComponent->AddInputVector(GetActorForwardVector() * InputValue);
	}
}

