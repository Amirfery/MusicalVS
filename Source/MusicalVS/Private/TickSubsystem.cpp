// Fill out your copyright notice in the Description page of Project Settings.


#include "TickSubsystem.h"

void UTickSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	bEnemyCanTick = true;
	CurrentTime = 0.0f;
	PreviousTime = 0.0f;
}

void UTickSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTickSubsystem::Tick(float DeltaTime)
{
	DefaultTickDelegate.Broadcast(DeltaTime);
	if (bEnemyCanTick)
		EnemyTickDelegate.Broadcast(DeltaTime);

	if (bIsTimeTicking)
	{
		CurrentTime += DeltaTime;
		if (CurrentTime - PreviousTime > 1.0f)
		{
			LevelTimerDelegate.Broadcast(FMath::FloorToInt(CurrentTime));
			PreviousTime = FMath::Floor(CurrentTime);
		}
	}
}

void UTickSubsystem::StartTimer()
{
	bIsTimeTicking = true;
}

void UTickSubsystem::StopTimer()
{
	bIsTimeTicking = false;
}
