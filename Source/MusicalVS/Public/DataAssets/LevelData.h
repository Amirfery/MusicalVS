// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelData.generated.h"

class UFMODEvent;
class UEnemyData;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnemyData> EnemyDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnChance = 0.2f;
};

USTRUCT(BlueprintType)
struct FSpawnPhase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartTimeInSeconds = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsInstant = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxEnemies = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemySpawnInfo> EnemiesSpawnInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnRate = 0.1f;
};

UCLASS(BlueprintType)
class MUSICALVS_API ULevelData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxLevelTimeInSeconds = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFMODEvent> WaveStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFMODEvent> EnemySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFMODEvent> BossSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSpawnPhase> SpawnPhases;
};
