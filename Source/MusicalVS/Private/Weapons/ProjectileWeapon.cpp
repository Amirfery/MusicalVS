// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "Engine/OverlapResult.h"
#include "Enemy.h"
#include "PoolSystem.h"
#include "DataAssets/AttackData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/Projectile.h"

#define ECC_Target ECC_GameTraceChannel2

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
	SearchRadius = AttackData->BaseRange;
	Damage = AttackData->BaseDamage;
	EffectTime = AttackData->BaseEffectTime;

	FString Marker = AttackData->AttackMarkers[0].ToString() + " Level";
	FmodAudioComp->SetParameter(FName(Marker), 0);

	FString Marker2 = AttackData->AttackMarkers[0].ToString();
	FmodAudioComp->SetParameter(FName(Marker2), 0);
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
	FAttackLevelStruct LevelUp = AttackData->LevelUps[Level];
	Damage = Damage + (Damage * LevelUp.DamageUp);
	EffectTime = EffectTime + (EffectTime * LevelUp.DamageUp);
	SearchRadius =SearchRadius + (SearchRadius * LevelUp.RangeUp);
	Level += 1;
	FString Marker = AttackData->AttackMarkers[0].ToString() + " Level";
	FmodAudioComp->SetParameter(FName(Marker), Level);

	FString Marker2 = AttackData->AttackMarkers[0].ToString();
	FmodAudioComp->SetParameter(FName(Marker2), Level);
	// FmodAudioComp->ParameterCache.Add(FName(TEXT("Piano Arp Level")), Level);
}

void AProjectileWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
	const FVector PlayerLoc = GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);

	const bool bHasHit = GetWorld()->OverlapMultiByChannel( Overlaps, PlayerLoc, FQuat::Identity, ECC_Target, Sphere );
	
	if (Overlaps.IsEmpty())
	{
		return;
	}
	AActor* NearestEnemy = nullptr;
	float MinDistSq = FLT_MAX;
	
	if (TargetType == ETargetType::Random)
	{
		NearestEnemy = Overlaps[FMath::RandRange(0, Overlaps.Num() - 1)].GetActor();
	}
	else
	{
		for (auto& Overlap : Overlaps)
		{
			AActor* Other = Overlap.GetActor();
			if (!Other || Other == this)
				continue;
		
			float DistSq = FVector::DistSquared(PlayerLoc, Other->GetActorLocation());

			if (DistSq < MinDistSq)
			{
				MinDistSq = DistSq;
				NearestEnemy = Other;
			}
		}
	}

	if (NearestEnemy)
	{
		DrawDebugLine(GetWorld(), PlayerLoc, NearestEnemy->GetActorLocation(), FColor::Red, false, 0.5f, 0, 2.f);

		SpawnProjectileAtEnemy(Cast<AEnemy>(NearestEnemy));
	}
}
