// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/BlessingSystem.h"
#include "FluteBlessing.generated.h"

UCLASS()
class MUSICALVS_API AFluteBlessing : public ABlessingSystem
{
	GENERATED_BODY()

public:
	AFluteBlessing();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
