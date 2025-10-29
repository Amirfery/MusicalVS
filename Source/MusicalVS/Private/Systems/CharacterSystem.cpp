// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSystem.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h" 
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

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

void ACharacterSystem::Init()
{
	
}

