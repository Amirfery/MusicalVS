// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "LevelManager.h"
#include "PoolManager.h"
#include "PoolSystem.h"
#include "TickSubsystem.h"
#include "DataAssets/EnemyData.h"
#include "Systems/CharacterSystem.h"

TObjectPtr<AEnemyManager> AEnemyManager::Instance = nullptr;

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentAliveEnemies = 0;
	bShouldSpawn = false;
	GetWorld()->GetSubsystem<ULevelManager>()->OnPhaseChanged.AddDynamic(this, &AEnemyManager::SetNewPhaseData);
}

void AEnemyManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentAliveEnemies > MaxEnemiesAlive)
		return;
	if (bIsInCooldown)
		return;
	if (!bShouldSpawn)
		return;

	float RandomFloat = FMath::RandRange(0.0f, MaxChance);
	
	float Offset = 0.0f;
	TObjectPtr<UEnemyData> ChosenEnemy;
	for (FEnemySpawnInfo Element : EnemyTypes)
	{
		if (RandomFloat <= Element.SpawnChance + Offset)
		{
			ChosenEnemy = Element.EnemyDataAsset;
			break;
		}
			Offset += Element.SpawnChance;
	}
	
	APoolItem* TempEnemy = APoolSystem::GetInstance()->PoolInstances[ChosenEnemy->PoolId]->GetNewItem();
	TempEnemy->SetFloatValues({MaxDistance});
	AEnemy* Enemy = Cast<AEnemy>(TempEnemy);
	Enemy->Initialize(ChosenEnemy);
	RelocateEnemy(TempEnemy);
	
	bIsInCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,
	                                       FTimerDelegate::CreateLambda([this]()
	                                       {
		                                       bIsInCooldown = false;
	                                       }), EnemySpawnInterval, false);
	CurrentAliveEnemies++;
	
	if (bIsFreeze)
	{
		Enemy->Freeze();
	}
	else
		Enemy->Unfreeze();
	AliveEnemies.Add(Enemy);
}

void AEnemyManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Instance = this;
}

void AEnemyManager::RelocateEnemy(APoolItem* Enemy) const
{
	FVector BoxExtent{};
	FVector BoxCenter{};
	Enemy->GetActorBounds(true, BoxCenter, BoxExtent);
	float Radius = FMath::Sqrt(FMath::RandRange(FMath::Pow(InnerCircleRadius, 2), FMath::Pow(OuterCircleRadius, 2)));
	float Angle = FMath::RandRange(0.0f, 2 * PI);
	Enemy->SetActorLocation(
		ACharacterSystem::GetCharacterInstance()->GetActorLocation() + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, BoxExtent.Z));
}

void AEnemyManager::EnemyDied(AEnemy* Enemy)
{
	CurrentAliveEnemies--;
	AliveEnemies.Remove(Enemy);
}

void AEnemyManager::FreezeEnemies()
{
	GetWorld()->GetTimerManager().ClearTimer(FreezeTimer);
	GetWorld()->GetSubsystem<UTickSubsystem>()->bEnemyCanTick = false;
	for (AEnemy* Actor : AliveEnemies)
	{
		Actor->Freeze();
	}
	bIsFreeze = true;
	GetWorld()->GetTimerManager().SetTimer(FreezeTimer, FTimerDelegate::CreateLambda([this]()
	{
		for (AEnemy* Actor : AliveEnemies)
		{
			Actor->Unfreeze();
		}
		GetWorld()->GetSubsystem<UTickSubsystem>()->bEnemyCanTick = true;
		bIsFreeze = false;
	}), 3.f, false);
}

void AEnemyManager::SetNewPhaseData(FSpawnPhase NewPhase)
{
	bShouldSpawn = true;
	if (NewPhase.bIsInstant)
	{
		switch (NewPhase.InstantType)
		{
		case EInstantType::Swarm:
			SpawnSwarm(NewPhase);
			return;
		case EInstantType::Random:
			SpawnRandomInstant(NewPhase);
			return;
		default:
			return;
		}
	}
	PhaseData = NewPhase;
	EnemySpawnInterval = PhaseData.SpawnRate;
	EnemyTypes.Empty();
	MaxChance = 0.0f;
	EnemyTypes = PhaseData.EnemiesSpawnInfo;
	for (FEnemySpawnInfo EnemySpawnInfo : PhaseData.EnemiesSpawnInfo)
	{
		MaxChance += EnemySpawnInfo.SpawnChance;
	}
}

void AEnemyManager::SpawnSwarm(FSpawnPhase SpawnPhase)
{
	for (FEnemySpawnInfo EnemySpawnInfo : SpawnPhase.EnemiesSpawnInfo)
	{
		const FVector ClusterCenter = GetRandomPointAroundPlayer(ACharacterSystem::GetCharacterInstance()->GetActorLocation(), InnerCircleRadius, OuterCircleRadius);
		for (int i = 0; i < EnemySpawnInfo.SpawnChance; i++)
		{
			APoolItem* TempEnemy = APoolSystem::GetInstance()->PoolInstances[EnemySpawnInfo.EnemyDataAsset->PoolId]->GetNewItem();
			TempEnemy->SetFloatValues({MaxDistance});
			AEnemy* Enemy = Cast<AEnemy>(TempEnemy);
			Enemy->Initialize(EnemySpawnInfo.EnemyDataAsset);
			Enemy->Speed *= 10;
			RelocateInstantEnemies(Enemy, ClusterCenter, 0.f, 1000.f);
			CurrentAliveEnemies++;
			AliveEnemies.Add(Enemy);
		}
	}
}

FVector AEnemyManager::GetRandomPointAroundPlayer(const FVector& PlayerLocation, float MinDis, float MaxDis)
{
	const float Angle = FMath::RandRange(0.f, 2.f * PI);
	const float Radius = FMath::Sqrt(
		FMath::RandRange(FMath::Square(MinDis), FMath::Square(MaxDis))
	);

	return PlayerLocation + FVector(
		FMath::Cos(Angle) * Radius,
		FMath::Sin(Angle) * Radius,
		0.f
	);
}

void AEnemyManager::RelocateInstantEnemies(AEnemy* Enemy, const FVector& Center, float InnerRadius, float OuterRadius)
{
	FVector BoxCenter, BoxExtent;
	Enemy->GetActorBounds(true, BoxCenter, BoxExtent);

	const float Angle = FMath::RandRange(0.f, 2.f * PI);
	const float Radius = FMath::RandRange(InnerRadius, OuterRadius);

	const FVector Offset(
		FMath::Cos(Angle) * Radius,
		FMath::Sin(Angle) * Radius,
		BoxExtent.Z
	);

	Enemy->SetActorLocation(Center + Offset, false, nullptr, ETeleportType::TeleportPhysics);
}

void AEnemyManager::SpawnRandomInstant(FSpawnPhase SpawnPhase)
{
	for (FEnemySpawnInfo EnemySpawnInfo : SpawnPhase.EnemiesSpawnInfo)
	{
		for (int i = 0; i < EnemySpawnInfo.SpawnChance; i++)
		{
			APoolItem* TempEnemy = APoolSystem::GetInstance()->PoolInstances[EnemySpawnInfo.EnemyDataAsset->PoolId]->GetNewItem();
			TempEnemy->SetFloatValues({MaxDistance});
			AEnemy* Enemy = Cast<AEnemy>(TempEnemy);
			Enemy->Initialize(EnemySpawnInfo.EnemyDataAsset);
			RelocateEnemy(Enemy);
			CurrentAliveEnemies++;
			AliveEnemies.Add(Enemy);
		}
	}
}
