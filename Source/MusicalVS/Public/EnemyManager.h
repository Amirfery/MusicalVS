// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyManager.generated.h"


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

private:
	bool bIsInCooldown;
	FTimerHandle CooldownTimer;
	TObjectPtr<APoolManager> EnemyPool;
};
