// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/LevelData.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelManager.generated.h"

class ULevelData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, FSpawnPhase, PhaseData);

UCLASS()
class MUSICALVS_API ULevelManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULevelData> LevelData;

	UPROPERTY(Transient)
	int32 CurrentPhase;

	UPROPERTY(Transient)
	FOnPhaseChanged OnPhaseChanged;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:
	UFUNCTION()
	void CheckLevelPhase(int32 CurrentTime);

	
	
};
