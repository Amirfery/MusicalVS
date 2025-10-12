// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolItem.h"


// Sets default values
APoolItem::APoolItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APoolItem::SetEnable(bool Enable)
{
	if (Enable)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		SetActorTickEnabled(true);
	} else
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}

// Called when the game starts or when spawned
void APoolItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoolItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APoolItem::Init(APoolManager* PoolManager)
{
	ParentPoolManager = PoolManager;
}

