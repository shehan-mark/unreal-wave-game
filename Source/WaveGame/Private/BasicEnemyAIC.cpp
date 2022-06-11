// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "NavigationSystem.h" 
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "DrawDebugHelpers.h"
#include "EnemyAIBase.h"
#include "Components/SphereComponent.h"

#include "EnemyAIBase.h"


ABasicEnemyAIC::ABasicEnemyAIC()
{
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	// this is what tells our behavior tree what to do
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	MovementForce = 1000.0f;
	bUseVelocityChange = false;
	MovementStoppingRadius = 100.0f;

	//Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABasicEnemyAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess called"));
	if (InPawn != nullptr)
	{
		CurrentPawn = Cast<AEnemyAIBase>(InPawn);
		
		// should be Target's location here
		NextPathPoint = FVector(0.0f, 0.0f, 0.0f);

		FString ObjectName = InPawn->GetName();
		AEnemyAIBase* BasicEnemy = Cast<AEnemyAIBase>(InPawn);
		if (BasicEnemy && BasicEnemy->BehaviorTreeRef != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess - InPawn Not Null"));
			BlackBoardComponent->InitializeBlackboard(*BasicEnemy->BehaviorTreeRef->BlackboardAsset);
			
			BlackBoardComponent->SetValueAsObject(FName("SelfActor"), InPawn);

			BehaviorTreeComponent->StartTree(*BasicEnemy->BehaviorTreeRef);
		}

	}

}

/*
	Direct movement to a destination without Force
*/
void ABasicEnemyAIC::MovePawnToLocation(FVector Location)
{
	//MoveToLocation(NextLocation);


	// BlackBoardComponent->SetValueAsVector(FName("TargetDestination"), NextLocation);
	// BlackBoardComponent->SetValueAsVector(FName("TargetDirection"), ForceDirection);
}

/*
	Handling movement by AI itself. Which is bad for my design
*/
void ABasicEnemyAIC::StartEnemyMovement(bool ShouldStart)
{
	CurrentPawn->ShouldStartMovement = ShouldStart;
}

FVector ABasicEnemyAIC::GetNextPathPoint()
{
	// I had to add Character.h header file in order for below function to understand that ACharacter is a type of Pawn.
	//UNavigationPath*  NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, CurrentPawn->GetActorLocation(), PlayerPawn);
	UNavigationPath*  NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, CurrentPawn->GetActorLocation(), FVector(0.0f, 0.0f, 0.0f));
	
	if (NavigationPath->PathPoints.Num() > 1)
	{
		// getting index 1 because 0 is always the current location.
		return NavigationPath->PathPoints[1];
	}

	return CurrentPawn->GetActorLocation();
}

void ABasicEnemyAIC::MoveAlongThePath()
{
	float DistanceToTarget = (CurrentPawn->GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget > MovementStoppingRadius)
	{
		NextPathPoint = GetNextPathPoint();
		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 26, FColor::Red, true, -1, 0, 2);
	}
	else
	{
		FVector ForceDirection = NextPathPoint - CurrentPawn->GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= 1000.f;

		DrawDebugDirectionalArrow(GetWorld(), CurrentPawn->GetActorLocation(), CurrentPawn->GetActorLocation() + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
		CurrentPawn->SphereComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}
}
