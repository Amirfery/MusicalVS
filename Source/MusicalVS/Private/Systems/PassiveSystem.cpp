// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/PassiveSystem.h"


// Sets default values
APassiveSystem::APassiveSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APassiveSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APassiveSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

