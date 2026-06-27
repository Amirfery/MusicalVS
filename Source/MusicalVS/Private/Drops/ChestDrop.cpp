// Fill out your copyright notice in the Description page of Project Settings.


#include "Drops/ChestDrop.h"

void UChestDrop::Drop(UWorld* World, FVector Location)
{
	FRotator SpawnRotator = FRotator::ZeroRotator;
	World->SpawnActor<AChest>(ChestClass, Location, SpawnRotator);
}
