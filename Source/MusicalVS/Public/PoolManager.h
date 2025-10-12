// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolManager.generated.h"

class APoolItem;

UCLASS()
class MUSICALVS_API APoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolManager();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void FreeItem(APoolItem* ItemToFree);

	UFUNCTION(BlueprintCallable)
	APoolItem* GetNewItem();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APoolItem> ActorToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PoolSize;

	TQueue<APoolItem*> FreeItems;

	int32 CurrentFreeItemsCount;

	
	

};
