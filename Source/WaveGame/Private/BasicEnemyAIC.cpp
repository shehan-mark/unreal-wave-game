// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BasicEnemy.h"

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
		//FString ObjectName = InPawn->GetName();
		ABasicEnemy* BasicEnemy = Cast<ABasicEnemy>(InPawn);
		if (BasicEnemy && BasicEnemy->BehaviorTreeRef != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABasicEnemyAIC::OnPossess - InPawn Not Null"));
			BlackBoardComponent->InitializeBlackboard(*BasicEnemy->BehaviorTreeRef->BlackboardAsset);
			BlackBoardComponent->SetValueAsVector(FName("TargetDestination"), FVector(0.f, 0.f, 0.f));

			BehaviorTreeComponent->StartTree(*BasicEnemy->BehaviorTreeRef);
		}
	}

}
