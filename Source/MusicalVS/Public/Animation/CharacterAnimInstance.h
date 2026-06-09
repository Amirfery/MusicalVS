// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ACharacterSystem;

UCLASS()
class MUSICALVS_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacterSystem> Character;

	UPROPERTY(BlueprintReadWrite)
	float Velocity;

	UPROPERTY(BlueprintReadWrite)
	int32 CharacterLevel = 0;
};
