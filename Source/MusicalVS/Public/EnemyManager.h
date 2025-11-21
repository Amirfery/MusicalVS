// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAssets/LevelData.h"
#include "EnemyManager.generated.h"


class UEnemyData;
class ACharacterSystem;
class APoolItem;
class AEnemy;
class APoolManager;

UCLASS(Blueprintable)
class MUSICALVS_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	AEnemyManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;

	void RelocateEnemy(APoolItem* Enemy) const;

	void EnemyDied(AEnemy* Enemy);

	void FreezeEnemies();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static AEnemyManager* GetInstance() {return Instance;}

	UFUNCTION()
	void SetNewPhaseData(FSpawnPhase NewPhase);

public:
	UPROPERTY(EditDefaultsOnly)
	FName EnemyPoolId;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FEnemySpawnInfo> EnemyTypes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InnerCircleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OuterCircleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxEnemiesAlive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnemySpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnPhase PhaseData;

private:
	bool bIsInCooldown;
	FTimerHandle CooldownTimer;
	UPROPERTY(Transient)
	TObjectPtr<APoolManager> EnemyPool;

	static TObjectPtr<AEnemyManager> Instance;

	UPROPERTY(EditDefaultsOnly)
	float MaxDistance;

	UPROPERTY(Transient)
	int32 CurrentAliveEnemies;

	UPROPERTY(Transient)
	TSet<TObjectPtr<AEnemy>> AliveEnemies;

	UPROPERTY(Transient)
	bool bIsFreeze;

	UPROPERTY(Transient)
	FTimerHandle FreezeTimer;

	UPROPERTY(Transient)
	float MaxChance;
};
