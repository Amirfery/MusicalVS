// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "MapComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSICALVS_API UMapComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	UMapComponent();

protected:
	virtual void PostInitProperties() override;

public:

	UFUNCTION(BlueprintCallable)
	static UMapComponent* GetMapInstance(){return Instance;}

public:
	static TObjectPtr<UMapComponent> Instance;
};
