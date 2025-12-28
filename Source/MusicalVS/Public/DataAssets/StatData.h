// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatData.generated.h"

UCLASS()
class MUSICALVS_API UCharacterStatData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bIsPlayerStats = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Recovery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Might;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Area;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsPlayerStats", EditConditionHides))
	float Luck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsPlayerStats", EditConditionHides))
	float Growth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsPlayerStats", EditConditionHides))
	float Greed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsPlayerStats", EditConditionHides))
	float Curse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIsPlayerStats", EditConditionHides))
	float Magnet;
};
