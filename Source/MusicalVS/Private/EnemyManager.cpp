// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "LevelManager.h"
#include "PoolManager.h"
#include "PoolSystem.h"
#include "TickSubsystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CharacterSystem.h"

TObjectPtr<AEnemyManager> AEnemyManager::Instance = nullptr;

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	EnemyPool = APoolSystem::GetInstance()->PoolInstances[EnemyPoolId];
	CurrentAliveEnemies = 0;
	GetWorld()->GetSubsystem<ULevelManager>()->OnPhaseChanged.AddDynamic(this, &AEnemyManager::SetNewPhaseData);
}

void AEnemyManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentAliveEnemies > MaxEnemiesAlive)
		return;
	if (bIsInCooldown)
		return;
	
	APoolItem* TempEnemy = EnemyPool->GetNewItem();
	TempEnemy->SetFloatValues({MaxDistance});
	AEnemy* Enemy = Cast<AEnemy>(TempEnemy);

	float RandomFloat = FMath::RandRange(0.0f, MaxChance);
	
	float Offset = 0.0f;
	for (FEnemySpawnInfo Element : EnemyTypes)
	{
		if (RandomFloat <= Element.SpawnChance + Offset)
		{
			Enemy->Initialize(Element.EnemyDataAsset);
			break;
		}
			Offset += Element.SpawnChance;
	}
	
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
	ACharacter* Player = ACharacterSystem::GetCharacterInstance();
	Enemy->SetActorLocation(
		Player->GetActorLocation() + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, BoxExtent.Z * 2));
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
