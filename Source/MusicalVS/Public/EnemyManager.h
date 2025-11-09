// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyManager.generated.h"


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

	void EnemyDied(APoolItem* Enemy);

public:
	UPROPERTY(EditDefaultsOnly)
	FName EnemyPoolId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InnerCircleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OuterCircleRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxEnemiesAlive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnemySpawnInterval;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static AEnemyManager* GetInstance() {return Instance;}

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
};
