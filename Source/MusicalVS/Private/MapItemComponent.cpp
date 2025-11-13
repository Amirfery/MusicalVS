// Fill out your copyright notice in the Description page of Project Settings.


#include "MapItemComponent.h"

#include "MapComponent.h"

UMapItemComponent::UMapItemComponent()
{
	bVisibleInSceneCaptureOnly = true;
}

void UMapItemComponent::BeginPlay()
{
	Super::BeginPlay();
	EnableInMap();
}

void UMapItemComponent::EnableInMap()
{
	UMapComponent::GetMapInstance()->ShowOnlyComponent(this);
}

void UMapItemComponent::DisableInMap()
{
	UMapComponent::GetMapInstance()->RemoveShowOnlyComponent(this);
}
