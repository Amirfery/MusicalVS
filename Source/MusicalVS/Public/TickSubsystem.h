// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TickSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MUSICALVS_API UTickSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override {return true;}
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UTickSubsystem, STATGROUP_Tickables); }

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickSubsystemDelegate, float, DeltaTime);
	UPROPERTY(BlueprintReadOnly)
	FTickSubsystemDelegate EnemyTickDelegate;

	UPROPERTY(BlueprintReadOnly)
	bool bEnemyCanTick;
	
	
};
