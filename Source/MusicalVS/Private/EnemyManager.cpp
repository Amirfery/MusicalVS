// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "PoolManager.h"
#include "PoolSystem.h"
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
}

void AEnemyManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentAliveEnemies > MaxEnemiesAlive)
		return;
	if (bIsInCooldown)
		return;
	ACharacter* Player = ACharacterSystem::GetCharacterInstance();
	
	float Radius = FMath::Sqrt(FMath::RandRange(FMath::Pow(InnerCircleRadius, 2), FMath::Pow(OuterCircleRadius, 2)));
	float Angle = FMath::RandRange(0.0f, 2 * PI);
	APoolItem* TempEnemy = EnemyPool->GetNewItem();
	TempEnemy->SetFloatValues({MaxDistance});
	TempEnemy->SetActorLocation(
		Player->GetActorLocation() + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 160.0f));
	bIsInCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,
	                                       FTimerDelegate::CreateLambda([this]()
	                                       {
		                                       bIsInCooldown = false;
	                                       }), EnemySpawnInterval, false);
	CurrentAliveEnemies++;
}

void AEnemyManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Instance = this;
}

void AEnemyManager::RelocateEnemy(APoolItem* Enemy) const
{
	float Radius = FMath::Sqrt(FMath::RandRange(FMath::Pow(InnerCircleRadius, 2), FMath::Pow(OuterCircleRadius, 2)));
	float Angle = FMath::RandRange(0.0f, 2 * PI);
	ACharacter* Player = ACharacterSystem::GetCharacterInstance();
	Enemy->SetActorLocation(
		Player->GetActorLocation() + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f));
}

void AEnemyManager::EnemyDied(APoolItem* Enemy)
{
	CurrentAliveEnemies--;
}
