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
#include "Kismet/KismetMathLibrary.h" 

#include "DrawDebugHelpers.h"
#include "EnemyAIBase.h"
#include "Components/SphereComponent.h"
//#include "Navigation/PathFollowingComponent.h"

#include "EnemyAIBase.h"

ABasicEnemyAIC::ABasicEnemyAIC()
{
	bLoggsEnabled = false;

	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	// this is what tells our behavior tree what to do
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	MovementStoppingRadius = 100.0f;
	bEnemyReachedPoint = false;
	TargetPointReachThreshold = 1.0f;

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
			//UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess - InPawn Not Null"));
			BlackBoardComponent->InitializeBlackboard(*BasicEnemy->BehaviorTreeRef->BlackboardAsset);
			
			BlackBoardComponent->SetValueAsObject(FName("SelfActor"), InPawn);
			BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint);

			BehaviorTreeComponent->StartTree(*BasicEnemy->BehaviorTreeRef);
		}

	}

}


void ABasicEnemyAIC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

/*
	Direct movement to a destination without Force
*/
void ABasicEnemyAIC::MovePawnToLocation(float DeltaSeconds)
{
	FVector CurrentLocation = CurrentPawn->GetActorLocation();
	FVector MovementDirection = (NextPathPoint - CurrentLocation);

	float MovementMag = GetEnemyToTargetPointLength();
	if (MovementMag <= TargetPointReachThreshold) // need to make this a variable 
	//if (CurrentLocation.Equals(NextPathPoint)) // need to make this a variable 
	{
		if (bLoggsEnabled)
		{
			UE_LOG(LogTemp, Error, TEXT("ABasicEnemyAIC::MovePawnToLocation Done Moving %f, %f, %f"), CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z);
		}
		bEnemyReachedPoint = true;
	}
	else
	{
		FVector Dir = MovementDirection;
		Dir.Normalize();

		FVector DeltaMovement = Dir * DeltaSeconds;
		FVector AddedSpeedLocaiton = DeltaMovement * 100.0f; // need to make this a variable 

		CurrentPawn->SetActorLocation(CurrentLocation + FVector(AddedSpeedLocaiton.X, AddedSpeedLocaiton.Y, 0.0f));
	}
}

void ABasicEnemyAIC::CheckAndUpdateNextPathPoint()
{
	if (GetEnemyToTargetPointLength() <= TargetPointReachThreshold)
	{
		FVector NextPoint = GetNextPathPoint();
		if (bLoggsEnabled)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::CheckAndUpdateNextPathPoint New Next Path Point %f, %f, %f"), NextPoint.X, NextPoint.Y, NextPoint.Z);
		}
		/*if ((NextLocation - NextPoint).Size() > 500.f)
		{
			NextPoint.Normalize();
			NextPoint *= 100.f;
		}*/
		NextPathPoint = NextPoint;
		BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint); // for BT use only
		UpdateEnemyLookRotation();

		// Now enemy has new location. So enemy has not reached to the given point
		bEnemyReachedPoint = false;
	}
}

FVector ABasicEnemyAIC::GetNextPathPoint()
{
	// I had to add Character.h header file in order for below function to understand that ACharacter is a type of Pawn.
	// UNavigationPath*  NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, CurrentPawn->GetActorLocation(), PlayerPawn);

	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	if (bLoggsEnabled)
	{
		UE_LOG(LogTemp, Error, TEXT("ABasicEnemyAIC::GetNextPathPoint Current Enemy location to get next path %f, %f, %f"), CurrentEnemyLocation.X, CurrentEnemyLocation.Y, CurrentEnemyLocation.Z);
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, CurrentEnemyLocation, FVector(0.0f, 0.0f, 0.0f));

	if (NavigationPath->PathPoints.Num() > 1)
	{
		// getting index 1 because 0 is always the current location.
		FVector PathPoint = FVector(NavigationPath->PathPoints[1].X, NavigationPath->PathPoints[1].Y, CurrentEnemyLocation.Z);
		return PathPoint;
	}

	return CurrentPawn->GetActorLocation();
}

/*
* Turn enemy's forward to target location
  https://forums.unrealengine.com/t/how-to-make-an-actor-rotate-to-look-in-a-certain-direction-in-c/144636/3
*/
void ABasicEnemyAIC::UpdateEnemyLookRotation()
{
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();

	if (bLoggsEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::UpdateEnemyLookRotation NextPathPoint Value %f, %f, %f"), NextPathPoint.X, NextPathPoint.Y, NextPathPoint.Z);
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::UpdateEnemyLookRotation Enemy Current Location %f, %f, %f"), CurrentEnemyLocation.X, CurrentEnemyLocation.Y, CurrentEnemyLocation.Z);
		DrawDebugSphere(GetWorld(), NextPathPoint, 10, 26, FColor::Green, true, 5, 0, 2);
		/* DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + NextLocation, 32, FColor::Yellow, false, 0.0f, 0, 1.0f); */
	}

	FVector MovementDirection = (NextPathPoint - CurrentEnemyLocation);
	MovementDirection.Normalize();

	//FRotator DummyRot = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, NextLocation);
	FRotator FinalRotaion = CurrentPawn->GetActorRotation() + MovementDirection.Rotation();
	CurrentPawn->SetActorRotation(FinalRotaion);

}


float ABasicEnemyAIC::GetEnemyToTargetPointLength()
{
	FVector NextLocation = BlackBoardComponent->GetValueAsVector(FName("NextPathPoint"));
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	FVector MovingDirection = (NextLocation - CurrentEnemyLocation);
	float MovementMag = MovingDirection.Size();
	if (bLoggsEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::GetEnemyToTargetPointLength MovementMag %f"), MovementMag);
	}
	return MovementMag;
}

/*
	Move Enemy using forces code sample
*/
//void ABasicEnemyAIC::MoveAlongThePath()
//{
//	FVector TargetLocation = FVector(0.0f, 0.0f, 0.0f);
//	FVector CurrentPawnLocation = CurrentPawn->GetActorLocation();
//	float DistanceToTarget = (TargetLocation - CurrentPawnLocation).Size();
//	if (DistanceToTarget < MovementStoppingRadius)
//	{
//		return;
//	}
//
//	float DistanceToNextPathPoint = (NextPathPoint - CurrentPawnLocation).Size();
//	if (DistanceToNextPathPoint < MovementStoppingRadius)
//	{
//		NextPathPoint = GetNextPathPoint();
//		DrawDebugSphere(GetWorld(), NextPathPoint, 10, 26, FColor::Red, true, -1, 0, 2);
//	}
//	else
//	{
//		FVector ForceDirection = (NextPathPoint - CurrentPawnLocation);
//		ForceDirection.Normalize();
//		ForceDirection *= MovementForce;
//
//		DrawDebugDirectionalArrow(GetWorld(), CurrentPawnLocation, CurrentPawnLocation + ForceDirection, 32, FColor::Yellow, false, 0.0f, 0, 1.0f);
//		CurrentPawn->SphereComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
//
//	}
//
//}
