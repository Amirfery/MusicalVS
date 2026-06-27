// Fill out your copyright notice in the Description page of Project Settings.


#include "Drops/PooledDrop.h"

#include "PoolItem.h"
#include "PoolManager.h"
#include "PoolSystem.h"

void UPooledDrop::Drop(UWorld* World, FVector Location)
{
	APoolSystem::PoolSystemInstance->PoolInstances[PoolName]->GetNewItem()->SetActorLocation(Location);
}
