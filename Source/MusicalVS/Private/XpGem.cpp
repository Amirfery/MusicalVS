// Fill out your copyright notice in the Description page of Project Settings.


#include "XpGem.h"

#include "Systems/CharacterSystem.h"


// Sets default values
AXpGem::AXpGem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AXpGem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXpGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXpGem::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
{
	PlayerSystem->AddXP(XpAmount);
	FreeItem();
}

