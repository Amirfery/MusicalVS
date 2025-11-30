// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TickSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickSubsystemDelegate, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelTimerDelegate, int32, CurrentTime);

UCLASS()
class MUSICALVS_API UTickSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override {return true;}
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UTickSubsystem, STATGROUP_Tickables); }

	UFUNCTION(BlueprintCallable)
	void StartTimer();
	UFUNCTION(BlueprintCallable)
	void StopTimer();

public:
	UPROPERTY(BlueprintReadOnly)
	FTickSubsystemDelegate EnemyTickDelegate;

	UPROPERTY(BlueprintReadOnly, BlueprintAssignable)
	FLevelTimerDelegate LevelTimerDelegate;

	UPROPERTY(BlueprintReadOnly)
	FTickSubsystemDelegate AnimationDelegate;

	UPROPERTY(Transient)
	bool bEnemyCanTick;

	UPROPERTY(BlueprintReadOnly)
	float CurrentTime;

	UPROPERTY(BlueprintReadOnly)
	float PreviousTime;

	UPROPERTY(Transient)
	bool bIsTimeTicking;
	
	UPROPERTY(BlueprintReadOnly)
	float AnimationCurrentTime;

	UPROPERTY(BlueprintReadOnly)
	float AnimationPreviousTime;

	UPROPERTY(Transient)
	bool bIsAnimationTimeTicking;

	UPROPERTY(EditDefaultsOnly)
	float AnimationFPS = 6.0f;
	
};
