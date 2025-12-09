// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyManager.h"
#include "PoolSystem.h"
#include "TickSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "DataAssets/EnemyData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
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
	Mesh = GetComponentByClass<UStaticMeshComponent>();
	CapsuleComponent = GetComponentByClass<UCapsuleComponent>();
	HealthComponent = GetComponentByClass<UHealthComponent>();
	DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	GetWorld()->GetSubsystem<UTickSubsystem>()->EnemyTickDelegate.AddDynamic(this, &AEnemy::TickEnemy);
	
}

void AEnemy::PostLoad()
{
	Super::PostLoad();
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickBatching = true;
	PrimaryActorTick.bRunOnAnyThread = true;
	SetActorTickEnabled(false); 
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsAlive)
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
	bIsAlive = false;
	APoolSystem::PoolSystemInstance->PoolInstances[XpPoolSystemName]->GetNewItem()->SetActorLocation(GetActorLocation());
	
	float RandomNum = FMath::RandRange(0.f, 100.f);
	if (RandomNum < 0.001)
	{
		APoolItem* Item = APoolSystem::PoolSystemInstance->PoolInstances[FName("PowerUp")]->GetNewItem();
		Item->SetFloatValues({static_cast<float>(FMath::RandRange(0, 1))});
		Item->SetActorLocation(GetActorLocation());
	}
	
	FreeItem();
	AEnemyManager::GetInstance()->EnemyDied(this);
}

void AEnemy::Initialize(UEnemyData* NewEnemyData)
{
	bIsAlive = true;
	EnemyData = NewEnemyData;
	HealthComponent->MaxHealth = EnemyData->Hp;
	HealthComponent->CurrentHealth = EnemyData->Hp;
	Mesh->SetStaticMesh(EnemyData->SkeletalMesh);
	DynMat = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, NewEnemyData->Material);
	DynMat->SetScalarParameterValue(FName("TimeOffset"), FMath::RandRange(1, 100));
	float temp;
	DynMat->GetScalarParameterValue(FName("TimeOffset"), temp);
	SetActorScale3D(EnemyData->Scale);
	Speed = EnemyData->Speed;
	Damage = EnemyData->Damage;
	FVector Min, Max;
	Mesh->GetLocalBounds(Min, Max);
	FVector  Bounds = (Max - Min) * 0.5;
	CapsuleComponent->SetCapsuleSize(Bounds.X, Bounds.Z);
}

void AEnemy::Freeze()
{
	CapsuleComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	// Mesh->Stop();
	DynMat->SetVectorParameterValue(FName("Tint"), FLinearColor(0,0,1));
}

void AEnemy::Unfreeze()
{
	// Mesh->Play(true);
	DynMat->SetVectorParameterValue(FName("Tint"), FLinearColor(1,1,1));
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

