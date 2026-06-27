// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drop.h"
#include "UObject/Object.h"
#include "PooledDrop.generated.h"

UCLASS()
class MUSICALVS_API UPooledDrop : public UDrop
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PoolName;
	
	virtual void Drop(UWorld* World, FVector Location) override;
};
