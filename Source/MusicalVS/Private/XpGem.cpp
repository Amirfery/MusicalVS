// Fill out your copyright notice in the Description page of Project Settings.


#include "XpGem.h"

#include "Systems/CharacterSystem.h"


// Sets default values
AXpGem::AXpGem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsMovingToPlayer = false;
	MoveSpeed = 1000.f;
	CollectDistance = 100.f;
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
	if (bIsMovingToPlayer && TargetPlayer)
	{
		FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation());
		float Distance = Direction.Size();

		if (Distance < CollectDistance)
		{
			CollidedWithPlayer(TargetPlayer);
		}
		else
		{
			Direction.Normalize();
			FVector NewLocation = GetActorLocation() + Direction * MoveSpeed * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
}

void AXpGem::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
{
	PlayerSystem->AddXP(XpAmount);
	FreeItem();
}

void AXpGem::Collect(ACharacterSystem* Player)
{
	TargetPlayer = Player;
	bIsMovingToPlayer = true;
}