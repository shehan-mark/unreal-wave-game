// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "NavigationSystem.h" 
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "HealthComponentBase.h"
#include "DrawDebugHelpers.h"
#include "BasicEnemyAIC.h"
#include "TurretHead.h"

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

	LifeSpanAfterDeath = 3.0f;
	DamageAmount = 5.0f;
}

// Called when the game starts or when spawned
void AEnemyAIBase::BeginPlay()
{
	Super::BeginPlay();
	EnemyStatus = EnemyState::IDLE;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAIBase::OnCollisionOverlap);
}

void AEnemyAIBase::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyAIBase::OnCollisionOverlap - Overlapped Actor %s"), *OtherActor->GetName());

	CurrentDamageTarget = Cast<ATurretHead>(OtherActor);
	if (CurrentDamageTarget)
	{
		EnemyStatus = EnemyState::ATTACK;
	}
}

// Called every frame
void AEnemyAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAIBase::SetEnemyStatus(EnemyState Status)
{
	EnemyStatus = Status;
}

EnemyState AEnemyAIBase::GetEnemyStatus()
{
	return EnemyStatus;
}

void AEnemyAIBase::DoDamage()
{
	ABasicEnemyAIC* Ctrl = Cast<ABasicEnemyAIC>(GetController());
	if (Ctrl)
	{
		//UE_LOG(LogTemp, Error, TEXT("AEnemyAIBase::DoDamage Damaging Player..."));
		UGameplayStatics::ApplyDamage(CurrentDamageTarget, DamageAmount, Ctrl, this, DamageType);
	}
}

void AEnemyAIBase::Die()
{
	AController* APC = GetController();
	if (APC)
	{
		SetEnemyStatus(EnemyState::DEAD);
		APC->UnPossess();
		SetLifeSpan(LifeSpanAfterDeath);
	}
}
