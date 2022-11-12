// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIBase.h"
#include "BasicProjectileDamage.h"
#include "DrawDebugHelpers.h"
#include "TurretHead.h"

// Sets default values
ABasicProjectile::ABasicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(16.0f);

	// Players can't walk on it
	SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = SphereComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
	MeshComponent->SetupAttachment(SphereComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = SphereComponent;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	EnemyKnockbackAmount = 200.0f;
}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnHit);

}

// Called every frame
void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//UE_LOG(LogTemp, Warning, TEXT("ABasicProjectile::OnHit %s"), *OtherActor->GetName());
	AEnemyAIBase* HitEnemy = Cast<AEnemyAIBase>(OtherActor);
	if (HitEnemy)
	{
		APlayerController* Ctrl = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (Ctrl)
		{
			//UE_LOG(LogTemp, Error, TEXT("ABasicProjectile::OnHit GOTTA DAMAGE"));
			UGameplayStatics::ApplyDamage(HitEnemy, 50.0f, Ctrl, this, DamageType);
		}

		FVector EnemyCurentLocation = OtherActor->GetActorLocation();
		FVector DamageCauserLocation = ProjectileOwner->GetActorLocation();

		FVector Dir = EnemyCurentLocation - FVector(0.0f, 0.0f, EnemyCurentLocation.Z);
		Dir.Normalize();

		Dir *= EnemyKnockbackAmount;
		FVector NewPos = EnemyCurentLocation + FVector(Dir.X, Dir.Y, 0.0f);

		//UE_LOG(LogTemp, Error, TEXT("ABasicProjectile::OnHit New Position %f, %f, %f"), NewPos.X, NewPos.Y, NewPos.Z);

		//FVector EndPosition = FVector(0.0f, 0.0f, 0.0f) * 20;
		//DrawDebugDirectionalArrow(GetWorld(), FVector(0.0f, 0.0f, EnemyCurentLocation.Z), FVector(Dir.X, Dir.Y, EnemyCurentLocation.Z), 5.0f, FColor::Red, true, 3.0f);
		DrawDebugDirectionalArrow(GetWorld(), FVector(0.0f, 0.0f, EnemyCurentLocation.Z), NewPos, 5.0f, FColor::Red, true, 3.0f);
		OtherActor->SetActorLocation(NewPos);
	}

}
