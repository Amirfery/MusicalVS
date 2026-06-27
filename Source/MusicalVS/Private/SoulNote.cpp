// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulNote.h"

#include "Systems/CharacterSystem.h"


// Sets default values
ASoulNote::ASoulNote()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsMovingToPlayer = false;
	MoveSpeed = 1000.f;
}

// Called when the game starts or when spawned
void ASoulNote::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoulNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMovingToPlayer && TargetPlayer)
	{
		FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation());

		Direction.Normalize();
		FVector NewLocation = GetActorLocation() + Direction * MoveSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void ASoulNote::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
{
	PlayerSystem->AddSoulNote(SoulNoteAmount);
	bIsMovingToPlayer = false;
	FreeItem();
}

void ASoulNote::Collect(ACharacterSystem* Player)
{
	TargetPlayer = Player;
	bIsMovingToPlayer = true;
}

