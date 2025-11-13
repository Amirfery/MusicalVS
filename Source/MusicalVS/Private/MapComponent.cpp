// Fill out your copyright notice in the Description page of Project Settings.


#include "MapComponent.h"

TObjectPtr<UMapComponent> UMapComponent::Instance;

UMapComponent::UMapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
}

void UMapComponent::PostInitProperties()
{
	Super::PostInitProperties();
	Instance = this;
}

