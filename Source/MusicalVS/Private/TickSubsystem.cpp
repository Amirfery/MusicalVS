// Fill out your copyright notice in the Description page of Project Settings.


#include "TickSubsystem.h"

void UTickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTickSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTickSubsystem::Tick(float DeltaTime)
{
	EnemyTickDelegate.Broadcast(DeltaTime);
}
