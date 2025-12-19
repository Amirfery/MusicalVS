// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/BlessingSystem.h"
#include "ThorBlessing.generated.h"

UCLASS()
class MUSICALVS_API AThorBlessing : public ABlessingSystem
{
	GENERATED_BODY()

public:
	AThorBlessing();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 10.f;

	float EffectTime = 0.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ApplyEffect_Implementation() override;
};
