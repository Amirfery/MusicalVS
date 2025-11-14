// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUps/Freeze.h"

#include "Enemy.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFreeze::AFreeze()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFreeze::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFreeze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFreeze::CollidedWithPlayer(ACharacterSystem* PlayerSystem)
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	for (AActor* Actor : FoundEnemies)
	{
		Actor->SetActorTickEnabled(false);
	}
	FreeItem();
}

