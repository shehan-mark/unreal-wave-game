// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyAIBase.h"


ABasicEnemyAIC::ABasicEnemyAIC()
{
	BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));

	// this is what tells our behavior tree what to do
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABasicEnemyAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess called"));
	if (InPawn != nullptr)
	{
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

void ABasicEnemyAIC::MovePawnToLocation(FVector Location)
{
	BlackBoardComponent->SetValueAsVector(FName("TargetDestination"), Location);
	MoveToLocation(Location);
}
