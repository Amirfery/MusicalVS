// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSystem.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h" 
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"



// Sets default values
ACharacterSystem::ACharacterSystem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterSystem::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(AutoAttackTimer, this, &ACharacterSystem::FindAndAttackNearestEnemy, 0.5f, true);
}

// Called every frame
void ACharacterSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterSystem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterSystem::Init(APoolManager* ProjectileManager)
{
	ProjectilePoolManager = ProjectileManager;
}

void ACharacterSystem::FindAndAttackNearestEnemy()
{
	const FVector PlayerLoc = GetActorLocation();
	constexpr float SearchRadius = 1000.f;

	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

	const bool bHasHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		PlayerLoc,
		FQuat::Identity,
		ECC_Pawn,
		Sphere
	);

	UE_LOG(LogTemp, Log, TEXT("[CharacterSystem] Checking for enemies..."));

	if (!bHasHit)
	{
		UE_LOG(LogTemp, Log, TEXT("No enemies found within radius %.1f"), SearchRadius);
		return;
	}

	AActor* NearestEnemy = nullptr;
	float MinDistSq = FLT_MAX;

	for (auto& Overlap : Overlaps)
	{
		AActor* Other = Overlap.GetActor();
		if (!Other || Other == this)
			continue;

		if (AEnemy* Enemy = Cast<AEnemy>(Other))
		{
			float DistSq = FVector::DistSquared(PlayerLoc, Enemy->GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Found enemy: %s | Distance: %.1f"), *Enemy->GetName(), FMath::Sqrt(DistSq));

			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				NearestEnemy = Enemy;
			}
		}
	}

	if (NearestEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nearest enemy: %s (%.1f units away)"), *NearestEnemy->GetName(), FMath::Sqrt(MinDistSq));

		DrawDebugLine(GetWorld(), PlayerLoc, NearestEnemy->GetActorLocation(), FColor::Red, false, 0.5f, 0, 2.f);

		SpawnProjectileAtEnemy(NearestEnemy);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Enemies overlapped, but none valid for attack."));
	}
}

void ACharacterSystem::SpawnProjectileAtEnemy(const AActor* TargetEnemy) const
{
	if (!ProjectilePoolManager || !TargetEnemy) return;

	APoolItem* Item = ProjectilePoolManager->GetNewItem();
	if (!Item) return;

	FVector SpawnLoc = GetActorLocation() + FVector(0, 0, 50.f);
	FVector TargetLoc = TargetEnemy->GetActorLocation();
	FVector Dir = (TargetLoc - SpawnLoc).GetSafeNormal();

	Item->SetActorLocation(SpawnLoc);
	FRotator SpawnRot = Dir.Rotation();
	Item->SetActorRotation(SpawnRot);
}

