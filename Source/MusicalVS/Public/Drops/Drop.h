// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Drop.generated.h"

/**
 * Base class for all droppable items
 */
UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class MUSICALVS_API UDrop : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClampMax=1.0))
	float Chance = 1.f;
	
	UFUNCTION(BlueprintCallable)
	virtual void Drop(UWorld* World, FVector Location);
};
