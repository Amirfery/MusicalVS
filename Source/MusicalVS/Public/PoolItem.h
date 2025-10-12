// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolManager.h"
#include "GameFramework/Actor.h"
#include "PoolItem.generated.h"

class APoolManager;

UCLASS()
class MUSICALVS_API APoolItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APoolItem();
	virtual void Tick(float DeltaTime) override;
	void Init(APoolManager* PoolManager);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void FreeItem(){ParentPoolManager->FreeItem(this);}

	void SetEnable(bool Enable);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<APoolManager> ParentPoolManager;
	
};
