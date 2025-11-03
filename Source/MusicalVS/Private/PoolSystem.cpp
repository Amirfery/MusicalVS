// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolSystem.h"

#include "PoolManager.h"

TObjectPtr<APoolSystem> APoolSystem::PoolSystemInstance;

// Sets default values
APoolSystem::APoolSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APoolSystem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PoolSystemInstance = this;
	for (auto Pool : Pools)
		PoolInstances.Add(Pool.Key, GetWorld()->SpawnActor<APoolManager>(Pool.Value, GetActorLocation(), FRotator::ZeroRotator));
}

// Called every frame
void APoolSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

