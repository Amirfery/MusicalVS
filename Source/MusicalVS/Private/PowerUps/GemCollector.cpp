// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/GemCollector.h"

#include "XpGem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGemCollector::AGemCollector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGemCollector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGemCollector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGemCollector::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
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

