// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "Engine/OverlapResult.h"
#include "Enemy.h"
#include "PoolSystem.h"
#include "DataAssets/AttackData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/Projectile.h"


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
	SearchRadius = WeaponData->BaseRange;
	Damage = WeaponData->BaseDamage;
	EffectTime = WeaponData->BaseEffectTime;

	FmodAudioComp->SetParameter(FName(TEXT("Piano Arp Level")), 0);
}

void AProjectileWeapon::SpawnProjectileAtEnemy(const AActor* TargetEnemy) const
{
	if (!APoolSystem::GetInstance()->PoolInstances[PoolId] || !TargetEnemy) return;

	APoolItem* Item = APoolSystem::GetInstance()->PoolInstances[PoolId]->GetNewItem();
	if (!Item) return;

	FVector SpawnLoc = GetActorLocation() + FVector(0, 0, 50.f);
	FVector TargetLoc = TargetEnemy->GetActorLocation();
	FVector Dir = (TargetLoc - SpawnLoc).GetSafeNormal();

	Item->SetActorLocation(SpawnLoc);
	FRotator SpawnRot = Dir.Rotation();
	Item->SetActorRotation(SpawnRot);
	AProjectile* Projectile = Cast<AProjectile>(Item);
	if (!Projectile) return;
	Projectile->InitProjectile(EffectTime, {1000, 0, 0}, Damage);
	
}

// Called every frame
void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeapon::Upgrade_Implementation()
{
	Super::Upgrade_Implementation();
	FAttackLevelStruct LevelUp = WeaponData->LevelUps[Level];
	Damage = Damage + (Damage * LevelUp.DamageUp);
	EffectTime = EffectTime + (EffectTime * LevelUp.DamageUp);
	SearchRadius =SearchRadius + (SearchRadius * LevelUp.RangeUp);
	Level += 1;
	FmodAudioComp->SetParameter(FName(TEXT("Piano Arp Level")), Level);
	// FmodAudioComp->ParameterCache.Add(FName(TEXT("Piano Arp Level")), Level);
}

void AProjectileWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
	const FVector PlayerLoc = GetActorLocation();

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
		// UE_LOG(LogTemp, Warning, TEXT("Other is: %s, Class: %s"), 
		// 	*Other->GetName(), 
		// 	*Other->GetClass()->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Other->GetName());
		if (!Other || Other == this)
			continue;

		if (AEnemy* Enemy = Cast<AEnemy>(Other))
		{
			// AEnemy* Enemy = Cast<AEnemy>(Other);
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
