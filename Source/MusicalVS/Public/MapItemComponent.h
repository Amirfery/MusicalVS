// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MapItemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSICALVS_API UMapItemComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

	UMapItemComponent();

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void EnableInMap();

	UFUNCTION(BlueprintCallable)
	void DisableInMap();
};
