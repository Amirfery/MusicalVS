// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/PowerUp.h"

#include "Enemy.h"
#include "EnemyManager.h"
#include "XpGem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APowerUp::APowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerUp::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
{
	switch (PowerUpType)
	{
	case EPowerUpType::GemCollector:
		CollectGems(PlayerSystem);
		break;
	case EPowerUpType::Freeze:
		FreezeEnemies(PlayerSystem);
		break;
	default:
		break;
	}
}

void APowerUp::CollectGems(ACharacterSystem* PlayerSystem)
{
	TArray<AActor*> FoundGems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AXpGem::StaticClass(), FoundGems);

	for (AActor* Actor : FoundGems)
	{
		if (AXpGem* Gem = Cast<AXpGem>(Actor))
		{
			Gem->Collect(PlayerSystem);
		}
	}
	FreeItem();
}

void APowerUp::FreezeEnemies(ACharacterSystem* PlayerSystem)
{
	AEnemyManager::GetInstance()->FreezeEnemies();
	FreeItem();
}

void APowerUp::SetFloatValues_Implementation(const TArray<float>& FloatValues)
{
	Super::SetFloatValues_Implementation(FloatValues);
	if (FloatValues[0] == 0)
	{
		PowerUpType = EPowerUpType::GemCollector;
	}
	else if (FloatValues[0] == 1)
	{
		PowerUpType = EPowerUpType::Freeze;
	}
}

