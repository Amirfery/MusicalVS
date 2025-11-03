// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolSystem.generated.h"

class APoolManager;

UCLASS()
class MUSICALVS_API APoolSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APoolSystem();

protected:
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static APoolSystem* GetInstance() {return PoolSystemInstance;}

	static TObjectPtr<APoolSystem> PoolSystemInstance;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<APoolManager>> Pools;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, TObjectPtr<APoolManager>> PoolInstances;
};
