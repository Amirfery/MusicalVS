// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyManager.h"
#include "PoolSystem.h"
#include "TickSubsystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CharacterSystem.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemy::Init_Implementation(APoolManager* PoolManager)
{
	Super::Init_Implementation(PoolManager);
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	USkeletalMeshComponent* Mesh = GetComponentByClass<USkeletalMeshComponent>();
	DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	GetWorld()->GetSubsystem<UTickSubsystem>()->EnemyTickDelegate.AddDynamic(this, &AEnemy::TickEnemy);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Initialized)
		return;
	FVector DistanceVec = PlayerCharacter->GetActorLocation() - GetActorLocation();
	float Distance = DistanceVec.Length();
	if (Distance > MaxDistance)
	{
		AEnemyManager::GetInstance()->RelocateEnemy(this);
	}
}

void AEnemy::Die()
{
	APoolSystem::PoolSystemInstance->PoolInstances[XpPoolSystemName]->GetNewItem()->SetActorLocation(GetActorLocation());
	
	float RandomNum = FMath::RandRange(0.f, 100.f);
	if (RandomNum < 20)
	{
		APoolItem* Item = APoolSystem::PoolSystemInstance->PoolInstances[FName("PowerUp")]->GetNewItem();
		Item->SetFloatValues({static_cast<float>(FMath::RandRange(0, 1))});
		Item->SetActorLocation(GetActorLocation());
	}
	
	FreeItem();
	AEnemyManager::GetInstance()->EnemyDied(this);
}

void AEnemy::Freeze()
{
	DynMat->SetVectorParameterValue(FName("Tint"), FLinearColor(0,0,1));
	SetActorTickEnabled(false);
}

void AEnemy::Unfreeze()
{
	DynMat->SetVectorParameterValue(FName("Tint"), FLinearColor(1,1,1));
	SetActorTickEnabled(true);
}

void AEnemy::SetFloatValues_Implementation(const TArray<float>& FloatValues)
{
	Super::SetFloatValues_Implementation(FloatValues);
	MaxDistance = FloatValues[0];
}

void AEnemy::TickEnemy(float DeltaTime)
{
	Tick(DeltaTime);
}

