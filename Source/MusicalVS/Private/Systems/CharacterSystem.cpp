// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSystem.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "DataAssets/AttackData.h"
#include "GameFramework/Character.h"
#include "Systems/WeapnSystem.h"

ACharacterSystem* ACharacterSystem::Instance = nullptr;

// Sets default values
ACharacterSystem::ACharacterSystem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Instance = this;
}

// Called when the game starts or when spawned
void ACharacterSystem::BeginPlay()
{
	Super::BeginPlay();
	Level = 0;
	NeededXpToLevelUp = 10;
	XP = 0;
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, this, &ACharacterSystem::FindAndAttackNearestEnemy, 0.5f, true);
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, [this, ]&ACharacterSystem::AoeAttack, 0.5f, true);
}

// Called every frame
void ACharacterSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterSystem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterSystem::AddWeapon(AWeapnSystem* Weapon)
{
	Weapons.Add(Weapon);
}

void ACharacterSystem::AddXP(int32 Amount)
{
	XP += Amount;
	const float XpProgress = XP / NeededXpToLevelUp;
	OnXPAdded.Broadcast(XpProgress);
	while (XP >= NeededXpToLevelUp)
	{
		XP -= NeededXpToLevelUp;
		Level++;
		// for (AWeapnSystem* Weapon : Weapons)
		// {
		// 	Weapon->Upgrade_Implementation();
		// }
		OnLevelUp.Broadcast();
		if (Level < 21)
		{
			NeededXpToLevelUp = Level * 10;
		}
		else if (Level > 20 && Level < 41)
		{
			NeededXpToLevelUp = 600 + Level * 13;
		}
		else if (Level > 40)
		{
			NeededXpToLevelUp = 2400 + Level * 16;
		}
	}
}

TArray<FAttackLevelStruct> ACharacterSystem::GetWeaponUpgrades()
{
	TArray<FAttackLevelStruct> LevelUps;
	for (AWeapnSystem* Weapon : Weapons)
	{
		if (Weapon->Level < Weapon->WeaponData->LevelUps.Num())
			LevelUps.Add(Weapon->WeaponData->LevelUps[Weapon->Level]);
	}
	if (LevelUps.Num() > 0)
		SetPaused(true);
	return LevelUps;
}

void ACharacterSystem::SetPaused(bool Paused)
{
	for (AWeapnSystem* Weapon : Weapons)
	{
		Weapon->SetPaused(Paused);
	}
}

void ACharacterSystem::Init()
{
}
