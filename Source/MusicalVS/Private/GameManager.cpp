// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	WeaponsDataTable = LoadObject<UDataTable>(
			nullptr,
			TEXT("/Game/Data/DT_Weapons.DT_Weapons")
		);
}
