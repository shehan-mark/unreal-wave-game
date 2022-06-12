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

	bShouldStopMoving = false;
	//Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABasicEnemyAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess called"));
	if (InPawn != nullptr)
	{
		CurrentPawn = Cast<AEnemyAIBase>(InPawn);
		
		// We use CurrentPawn Current location as when the MoveAlong task starts it thinks that AI pawn has reached one NextPathPoint
		// Therefore it should request a new NextPathPoint
		NextPathPoint = CurrentPawn->GetActorLocation();

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
	FVector TargetLocation = FVector(0.0f, 0.0f, 0.0f);
	FVector CurrentPawnLocation = CurrentPawn->GetActorLocation();
	float DistanceToTarget = (TargetLocation - CurrentPawnLocation).Size();
	if (DistanceToTarget < MovementStoppingRadius)
	{
		bShouldStopMoving = true;
		return;
	}

	float DistanceToNextPathPoint = (NextPathPoint - CurrentPawnLocation).Size();
	if (DistanceToNextPathPoint < MovementStoppingRadius)
	{
		NextPathPoint = GetNextPathPoint();
		//DrawDebugSphere(GetWorld(), NextPathPoint, 10, 26, FColor::Red, true, -1, 0, 2);
	}
	else
	{
		FVector ForceDirection = (NextPathPoint - CurrentPawnLocation);
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;

		//DrawDebugDirectionalArrow(GetWorld(), CurrentPawnLocation, CurrentPawnLocation + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
		CurrentPawn->SphereComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}

}
