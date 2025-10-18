﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class MUSICALVS_API AEnemy : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();
	virtual void Init(APoolManager* PoolManager) override;
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> PlayerCharacter;	
};
