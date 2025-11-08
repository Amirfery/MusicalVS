// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

#include "Enemy.h"
#include "PoolManager.h"
#include "PoolSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	EnemyPool = APoolSystem::GetInstance()->PoolInstances[EnemyPoolId];
}

void AEnemyManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsInCooldown)
		return;
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float Radius = FMath::Sqrt(FMath::RandRange(FMath::Pow(InnerCircleRadius, 2), FMath::Pow(OuterCircleRadius, 2)));
	float Angle = FMath::RandRange(0.0f, 2 * PI);
	APoolItem* TempEnemy = EnemyPool->GetNewItem();
	TempEnemy->SetActorLocation(
		Player->GetActorLocation() + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f));
	bIsInCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,
	                                       FTimerDelegate::CreateLambda([this]()
	                                       {
		                                       bIsInCooldown = false;
	                                       }), EnemySpawnInterval, false);
}
