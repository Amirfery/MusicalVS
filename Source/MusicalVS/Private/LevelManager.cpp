// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "GameManager.h"
#include "TickSubsystem.h"
#include "DataAssets/LevelData.h"
#include "Infrastructure/GenericStructs.h"


class UGameManager;

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentPhase = -1;
}

void ULevelManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LevelData = Cast<UGameManager>(GetWorld()->GetGameInstance())->LevelsDataTable->FindRow<FLevelRow>(FName("Level0"),TEXT("Level lookup"))->LevelData;
	GetWorld()->GetSubsystem<UTickSubsystem>()->LevelTimerDelegate.AddDynamic(this, &ULevelManager::CheckLevelPhase);
}

void ULevelManager::CheckLevelPhase(int32 CurrentTime)
{
	if (CurrentPhase == LevelData->SpawnPhases.Num() - 1)
		return;
	
	if (CurrentTime >= LevelData->SpawnPhases[CurrentPhase + 1].StartTimeInSeconds)
	{
		CurrentPhase += 1;
		OnPhaseChanged.Broadcast(LevelData->SpawnPhases[CurrentPhase]);
	}
}
