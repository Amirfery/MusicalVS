// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "Engine/OverlapResult.h"
#include "Enemy.h"


// Sets default values
AProjectileWeapon::AProjectileWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileWeapon::SpawnProjectileAtEnemy(const AActor* TargetEnemy) const
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

// Called every frame
void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
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
	
	if (!bHasHit)
	{
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
			// UE_LOG(LogTemp, Log, TEXT("Found enemy: %s | Distance: %.1f"), *Enemy->GetName(), FMath::Sqrt(DistSq));

			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				NearestEnemy = Enemy;
			}
		}
	}

	if (NearestEnemy)
	{
		DrawDebugLine(GetWorld(), PlayerLoc, NearestEnemy->GetActorLocation(), FColor::Red, false, 0.5f, 0, 2.f);

		SpawnProjectileAtEnemy(NearestEnemy);
	}
}