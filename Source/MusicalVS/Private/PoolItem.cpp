// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolItem.h"

#include "Compression/lz4.h"


// Sets default values
APoolItem::APoolItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APoolItem::FreeItem_Implementation()
{
	//Initialized = false;
	if (ParentPoolManager)
	{
		ParentPoolManager->FreeItem(this);
	}
	
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
		SetActorLocation(ParentPoolManager->GetActorLocation());
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}

void APoolItem::SetFloatValues_Implementation(const TArray<float>& FloatValues)
{
	return;
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

void APoolItem::Init_Implementation(APoolManager* PoolManager)
{
	ParentPoolManager = PoolManager;
	Initialized = true;
}
