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
	APoolItem();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent)
	void Init(APoolManager* PoolManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FreeItem();

	void SetEnable(bool Enable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFloatValues(const TArray<float>& FloatValues);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TObjectPtr<APoolManager> ParentPoolManager;

	UPROPERTY(BlueprintReadOnly)
	bool Initialized;
};
