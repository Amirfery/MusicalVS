// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolManager.h"

#include "PoolItem.h"

// Sets default values
APoolManager::APoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

APoolItem* APoolManager::GetNewItem()
{
	APoolItem* Item = nullptr;
	if (!FreeItems.IsEmpty())
	{
		CurrentFreeItemsCount--;
		FreeItems.Dequeue(Item);
	} else
	{
		Item = GetWorld()->SpawnActor<APoolItem>(ActorToSpawn, FTransform::Identity);
		Item->Init(this);
	}
	Item->SetEnable(true);
	return Item;
}

// Called when the game starts or when spawned
void APoolManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentFreeItemsCount = 0;
	
}

// Called every frame
void APoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoolManager::FreeItem(APoolItem* ItemToFree)
{
	if (CurrentFreeItemsCount >= PoolSize)
	{
		ItemToFree->Destroy();
	} else
	{
		ItemToFree->SetEnable(false);
		FreeItems.Enqueue(ItemToFree);
		CurrentFreeItemsCount++;
	}
}

