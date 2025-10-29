// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackData.generated.h"

class UFMODEvent;

USTRUCT(BlueprintType)
struct FAttackLevelStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 1))
	float DamageUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 1))
	float RangeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 1))
	float EffectTimeUp;
};
UCLASS(BlueprintType)
class MUSICALVS_API UAttackData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> AttackMarkers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UFMODEvent> SoundEvent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseEffectTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttackLevelStruct> LevelUps;
	
};
