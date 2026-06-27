// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drop.h"
#include "Chests/Chest.h"
#include "UObject/Object.h"
#include "ChestDrop.generated.h"

UCLASS()
class MUSICALVS_API UChestDrop : public UDrop
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AChest> ChestClass;
	
	virtual void Drop(UWorld* World, FVector Location) override;
};
