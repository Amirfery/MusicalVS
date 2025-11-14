// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "GemCollector.generated.h"

class ACharacterSystem;

UCLASS()
class MUSICALVS_API AGemCollector : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGemCollector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CollidedWithPlayer(ACharacterSystem* PlayerSystem);
};
