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
#include "TimerManager.h"

#include "EnemyAIBase.h"
#include "TurretHead.h"


ABasicEnemyAIC::ABasicEnemyAIC()
{
	bLoggsEnabled = false;

	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	// this is what tells our behavior tree what to do
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	MovementStoppingRadius = 200.0f;
	TargetPointReachThreshold = 1.0f;
	EnemyMovementSpeed = 100.0f;

	bShouldStopMovement = false;
	bEnemyReachedCurrentTarget = false;
}

void ABasicEnemyAIC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BlackBoardComponent->SetValueAsEnum(FName("EnemyState"), (uint8)CurrentPawn->GetEnemyStatus());
}

void ABasicEnemyAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess called"));
	if (InPawn != nullptr)
	{
		CurrentPawn = Cast<AEnemyAIBase>(InPawn);
		
		CurrentGoalLocation = FVector(0.0f, 0.0f, CurrentPawn->GetActorLocation().Z);
		
		/*
		*	initial next path point is the current location
		*	therefore in the first move check AIC think it already reached the NextPathPoint
		*	and start calculating the new pathpoint
		*/
		NextPathPoint = CurrentPawn->GetActorLocation();

		if (CurrentPawn && CurrentPawn->BehaviorTreeRef != nullptr)
		{
			BlackBoardComponent->InitializeBlackboard(*CurrentPawn->BehaviorTreeRef->BlackboardAsset);
			
			BlackBoardComponent->SetValueAsObject(FName("SelfActor"), InPawn);
			BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint);
			BlackBoardComponent->SetValueAsBool(FName("ShouldStopMovement"), bShouldStopMovement);
			
			BehaviorTreeComponent->StartTree(*CurrentPawn->BehaviorTreeRef);
		}

	}
}

void ABasicEnemyAIC::UpdateCurrentGoalLocation()
{
	// for now this will be a zero vector. But if are going to have more turrets. This will be helpful
	CurrentGoalLocation = FVector::ZeroVector;
}

void ABasicEnemyAIC::FindAndMakeTarget()
{
	/*
	* what happens after wait in BT 
	* need find next/ closest target logic here
	* 
	*/
	if (!CurrentGoalLocation.IsZero() && CurrentGoalLocation != CurrentPawn->GetActorLocation() && !bShouldStopMovement)
	{
		NextPathPoint = GetNextPathPoint();
		BlackBoardComponent->SetValueAsVector(FName("NextPathPoint"), NextPathPoint);

		UpdateEnemyLookRotation();
		bEnemyReachedCurrentTarget = false;
		CurrentPawn->SetEnemyStatus(EnemyState::MOVING);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABasicEnemyAIC::FindAndMakeTarget ELSE Part"));

		// otherwise just wonder since we cant even find a target
	}
}

/*
*  Turn enemy's forward to target location
*  https://forums.unrealengine.com/t/how-to-make-an-actor-rotate-to-look-in-a-certain-direction-in-c/144636/3
*/
void ABasicEnemyAIC::UpdateEnemyLookRotation()
{
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	
	/*
	*  this FindLookAtRotation function gets the job done. But I want to know how to do it manually which we will do later
	*/
	FVector NextLocation = NextPathPoint;
	if (CurrentPawn->GetEnemyStatus() == EnemyState::ATTACK)
	{
		FVector _CurrentDamageTargetLocation = CurrentPawn->CurrentDamageTarget->GetActorLocation();
		NextLocation = FVector(_CurrentDamageTargetLocation.X, _CurrentDamageTargetLocation.Y, CurrentEnemyLocation.Z);
	}
	FRotator FinalRotation = UKismetMathLibrary::FindLookAtRotation(CurrentEnemyLocation, NextLocation);
	CurrentPawn->SetActorRotation(FinalRotation);
}

void ABasicEnemyAIC::StartAttack()
{
	GetWorldTimerManager().SetTimer(TimerHandle_EnemyAttack, this, &ABasicEnemyAIC::AttackTarget, CurrentPawn->AttackRate, true, 0.0f);
}

void ABasicEnemyAIC::AttackTarget()
{
	bool bCloseEnoughToDoDamage = GetEnemyToTargetPointLength(CurrentGoalLocation) <= MovementStoppingRadius;
	/*TurretState NowState = CurrentPawn->CurrentDamageTarget->GetTurretStatus();
	FString Str = UEnum::GetValueAsString(NowState);
	UE_LOG(LogTemp, Error, TEXT("ATTACKING TARGET %s"), *Str);*/
	if (bCloseEnoughToDoDamage && CurrentPawn->CurrentDamageTarget->GetTurretStatus() != TurretState::DEAD)
	{
		CurrentPawn->DoDamage();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_EnemyAttack);
		CurrentPawn->SetEnemyStatus(EnemyState::IDLE);
		bEnemyReachedCurrentTarget = false;
		bShouldStopMovement = false;
	}
}

/*
	
*/
void ABasicEnemyAIC::MovePawnToLocation(float DeltaSeconds)
{
	FVector CurrentLocation = CurrentPawn->GetActorLocation();

	float MovementMag = GetEnemyToTargetPointLength(NextPathPoint);
	if (MovementMag <= TargetPointReachThreshold)
	{
		if (CurrentPawn->GetEnemyStatus() == EnemyState::ATTACK)
		{
			bEnemyReachedCurrentTarget = true;
			bShouldStopMovement = true;
			BlackBoardComponent->SetValueAsBool(FName("ShouldStopMovement"), bShouldStopMovement);
			UpdateEnemyLookRotation();	
		}
		else
		{
			bEnemyReachedCurrentTarget = true;
			CurrentPawn->SetEnemyStatus(EnemyState::IDLE);
		}
	}
	else
	{
		/*if (CurrentPawn->GetEnemyStatus() != EnemyState::MOVING)
			CurrentPawn->SetEnemyStatus(EnemyState::MOVING);*/
		
		FVector MovementDirection = (NextPathPoint - CurrentLocation);

		FVector Dir = MovementDirection;
		Dir.Normalize();

		FVector DeltaMovement = Dir * DeltaSeconds;
		FVector AddedSpeedLocaiton = DeltaMovement * EnemyMovementSpeed;

		CurrentPawn->SetActorLocation(CurrentLocation + FVector(AddedSpeedLocaiton.X, AddedSpeedLocaiton.Y, 0.0f));
	}
}

FVector ABasicEnemyAIC::GetNextPathPoint()
{
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	/*
	*	I had to add Character.h header file in order for below function to understand that ACharacter is a type of Pawn.
	*	UNavigationPath*  NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, CurrentPawn->GetActorLocation(), PlayerPawn);
	*
	*/
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, CurrentEnemyLocation, CurrentGoalLocation);

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

float ABasicEnemyAIC::GetEnemyToTargetPointLength(FVector TargetPoint)
{
	FVector CurrentEnemyLocation = CurrentPawn->GetActorLocation();
	FVector MovingDirection = (TargetPoint - CurrentEnemyLocation);
	float MovementMag = MovingDirection.Size();

	return MovementMag;
}
