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
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

#include "EnemyAIBase.h"
//#include "Navigation/PathFollowingComponent.h"

#include "EnemyAIBase.h"

ABasicEnemyAIC::ABasicEnemyAIC()
{
	bLoggsEnabled = false;

	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	// this is what tells our behavior tree what to do
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	MovementStoppingRadius = 100.0f;
	TargetPointReachThreshold = 1.0f;
	EnemyMovementSpeed = 100.0f;

	bShouldStopMovement = false;
	bEnemyReachedPoint = false;
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
		if (CurrentPawn && CurrentPawn->BehaviorTreeRef != nullptr)
		{
			BlackBoardComponent->InitializeBlackboard(*CurrentPawn->BehaviorTreeRef->BlackboardAsset);
			
			BlackBoardComponent->SetValueAsObject(FName("SelfActor"), InPawn);
			BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint);
			BlackBoardComponent->SetValueAsBool(FName("ShouldStopMovement"), bShouldStopMovement);
			// set enemy status in the behavior tree as well

			/* start behavior tree execution */
			BehaviorTreeComponent->StartTree(*CurrentPawn->BehaviorTreeRef);
		}

	}
}


void ABasicEnemyAIC::AttackTarget()
{
	bool bCloseEnoughToDoDamage = GetEnemyToTargetPointLength(FVector(0.0f, 0.0f, CurrentPawn->GetActorLocation().Z)) <= MovementStoppingRadius;
	if (bCloseEnoughToDoDamage)
	{
		// attack
		CurrentPawn->DoDamage();
	}
	else
	{
		CurrentPawn->SetEnemyStatus(EnemyState::IDLE);
		bEnemyReachedPoint = false;
		bShouldStopMovement = false;
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

	FVector UltimateGoalLocation = FVector(0.0f, 0.0f, CurrentLocation.Z);

	float MovementMag = GetEnemyToTargetPointLength(NextPathPoint);
	if (MovementMag <= TargetPointReachThreshold)
	{
		if (bLoggsEnabled)
		{
			UE_LOG(LogTemp, Error, TEXT("ABasicEnemyAIC::MovePawnToLocation Enemy To Ultimate Goal Lenght %f"), GetEnemyToTargetPointLength(UltimateGoalLocation));
		}
		bEnemyReachedPoint = true;
		if (CurrentPawn->GetEnemyStatus() == EnemyState::ATTACK)
		{
			// enemy has reached to both point and ultimate goal
			bShouldStopMovement = true;
			BlackBoardComponent->SetValueAsBool(FName("ShouldStopMovement"), bShouldStopMovement);

			UpdateEnemyLookRotation();
			
		}
	}
	else
	{
		if (CurrentPawn->GetEnemyStatus() != EnemyState::MOVING)
			CurrentPawn->SetEnemyStatus(EnemyState::MOVING);

		FVector Dir = MovementDirection;
		Dir.Normalize();

		FVector DeltaMovement = Dir * DeltaSeconds;
		FVector AddedSpeedLocaiton = DeltaMovement * EnemyMovementSpeed;

		CurrentPawn->SetActorLocation(CurrentLocation + FVector(AddedSpeedLocaiton.X, AddedSpeedLocaiton.Y, 0.0f));
	}
}

void ABasicEnemyAIC::CheckAndUpdateNextPathPoint()
{
	if (GetEnemyToTargetPointLength(NextPathPoint) <= TargetPointReachThreshold)
	{
		if (bLoggsEnabled)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::CheckAndUpdateNextPathPoint New Next Path Point %f, %f, %f"), NextPoint.X, NextPoint.Y, NextPoint.Z);
		}
		NextPathPoint = GetNextPathPoint();
		UpdateEnemyLookRotation();
		BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint);

		// Now enemy has new location. So enemy has not reached to the given point
		bEnemyReachedPoint = false;
		CurrentPawn->SetEnemyStatus(EnemyState::MOVING);
		BlackBoardComponent->SetValueAsEnum(FName("EnemyState"), (uint8)CurrentPawn->GetEnemyStatus());
	}
}

FVector ABasicEnemyAIC::GetNextPathPoint()
{
	// I had to add Character.h header file in order for below function to understand that ACharacter is a type of Pawn.
	// UNavigationPath*  NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, CurrentPawn->GetActorLocation(), PlayerPawn);
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	if (bLoggsEnabled)
	{
		//UE_LOG(LogTemp, Error, TEXT("ABasicEnemyAIC::GetNextPathPoint Current Enemy location to get next path %f, %f, %f"), CurrentEnemyLocation.X, CurrentEnemyLocation.Y, CurrentEnemyLocation.Z);
	}

	// the target vector should be dynamic not hardcoded like this. Look into this later?
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, CurrentEnemyLocation, FVector(0.0f, 0.0f, 0.0f));

	if (NavigationPath->PathPoints.Num() > 1)
	{
		FVector PathPoint = CurrentEnemyLocation;
		for (int i = 1; i < NavigationPath->PathPoints.Num(); i++)
		{
			PathPoint = FVector(NavigationPath->PathPoints[i].X, NavigationPath->PathPoints[i].Y, CurrentEnemyLocation.Z);
			if (GetEnemyToTargetPointLength(PathPoint) > TargetPointReachThreshold)
			{
				break;
			}
		}

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

	/*if (bLoggsEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::UpdateEnemyLookRotation NextPathPoint Value %f, %f, %f"), NextPathPoint.X, NextPathPoint.Y, NextPathPoint.Z);
		UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::UpdateEnemyLookRotation Enemy Current Location %f, %f, %f"), CurrentEnemyLocation.X, CurrentEnemyLocation.Y, CurrentEnemyLocation.Z);
		DrawDebugSphere(GetWorld(), CurrentEnemyLocation, 10, 26, FColor::Yellow, true, 5, 0, 2);
		DrawDebugSphere(GetWorld(), NextPathPoint, 10, 26, FColor::Green, true, 5, 0, 2);
	}*/
	
	UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::UpdateEnemyLookRotation Getting Called"));

	/*
		this FindLookAtRotation function gets the job done. But I want to know how to do it manually which we will do later
	*/
	FVector NextLocation = NextPathPoint;
	if (CurrentPawn->GetEnemyStatus() == EnemyState::ATTACK)
	{
		FVector _CurrentDamageTargetLocation = CurrentPawn->CurrentDamageTarget->GetActorLocation();
		NextLocation = FVector(_CurrentDamageTargetLocation.X, _CurrentDamageTargetLocation.Y, NextPathPoint.Z);
	}
	FRotator FinalRotation = UKismetMathLibrary::FindLookAtRotation(CurrentEnemyLocation, NextLocation);
	CurrentPawn->SetActorRotation(FinalRotation);

}


float ABasicEnemyAIC::GetEnemyToTargetPointLength(FVector TargetPoint)
{
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	FVector MovingDirection = (TargetPoint - CurrentEnemyLocation);
	float MovementMag = MovingDirection.Size();
	if (bLoggsEnabled)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::GetEnemyToTargetPointLength MovementMag %f"), MovementMag);
	}
	return MovementMag;
}


#pragma region PREVIOUS LOGIC
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

#pragma endregion