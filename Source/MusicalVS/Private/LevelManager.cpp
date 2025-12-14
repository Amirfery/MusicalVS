// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "GameManager.h"
#include "TickSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/LevelData.h"
#include "Infrastructure/GenericStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/CharacterSystem.h"
#include "Systems/WeapnSystem.h"


class UGameManager;

void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentPhase = -1;
	bPlayWaveSound = false;
}

void ULevelManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LevelData = Cast<UGameManager>(GetWorld()->GetGameInstance())->LevelsDataTable->FindRow<FLevelRow>(FName("Level0"),TEXT("Level lookup"))->LevelData;
	GetWorld()->GetSubsystem<UTickSubsystem>()->LevelTimerDelegate.AddDynamic(this, &ULevelManager::CheckLevelPhase);
	GetWorld()->GetSubsystem<UTickSubsystem>()->DefaultTickDelegate.AddDynamic(this, &ULevelManager::Tick);
}

void ULevelManager::CheckLevelPhase(int32 CurrentTime)
{
	if (CurrentPhase == LevelData->SpawnPhases.Num() - 1)
		return;

	if (CurrentTime >= LevelData->SpawnPhases[CurrentPhase + 1].StartTimeInSeconds)
	{
		CurrentPhase += 1;
		OnPhaseChanged.Broadcast(LevelData->SpawnPhases[CurrentPhase]);
		if (CurrentPhase > 0 && !LevelData->SpawnPhases[CurrentPhase].bIsInstant)
			bPlayWaveSound = true;
	}
}

void ULevelManager::Tick(float DeltaTime)
{
	if (bPlayWaveSound)
	{
		float per = ACharacterSystem::GetCharacterInstance()->MainWeapon->GetEventPercentage();

		if (per < 0.1f)
		{
			UFMODBlueprintStatics::PlayEventAttached(LevelData->WaveStartSound, ACharacterSystem::GetCharacterInstance()->GetCapsuleComponent(), FName(TEXT("Wave Sound")), FVector::ZeroVector, EAttachLocation::Type::SnapToTarget,
				false, true, true);
			bPlayWaveSound = false;
		}
	}
}
