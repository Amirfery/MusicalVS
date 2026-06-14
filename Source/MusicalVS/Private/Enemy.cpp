// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyManager.h"
#include "PoolSystem.h"
#include "TickSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/EnemyStatComponent.h"
#include "DataAssets/EnemyData.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Systems/CharacterSystem.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Stats = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stats"));
	Tags.Add(FName("Enemy"));
}

void AEnemy::Init_Implementation(APoolManager* PoolManager)
{
	Super::Init_Implementation(PoolManager);
	PlayerCharacter = Cast<ACharacterSystem>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Mesh = GetComponentByClass<UStaticMeshComponent>();
	CapsuleComponent = GetComponentByClass<UCapsuleComponent>();
	DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	GetWorld()->GetSubsystem<UTickSubsystem>()->EnemyTickDelegate.AddDynamic(this, &AEnemy::TickEnemy);
	Stats->OnEnemyDied.AddDynamic(this, &AEnemy::Die);
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AEnemy::OnCapsuleHit);
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

void AEnemy::TryDropChest() const
{
	if (EnemyData->PotentialChests.IsEmpty())
		return;
	
	float RandomFloat = FMath::RandRange(0.0f, 100.0f);
	float CumulativeChance = 0.0;
	for (const FChestDrop& ChestDrop : EnemyData->PotentialChests)
	{
		CumulativeChance += ChestDrop.DropChance;
		if (RandomFloat <= CumulativeChance)
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRotator = FRotator::ZeroRotator;
			GetWorld()->SpawnActor<AChest>(ChestDrop.ChestClass, SpawnLocation, SpawnRotator);
			break;
		}
	}
}

void AEnemy::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == PlayerCharacter || OtherActor->ActorHasTag(FName("Enemy")))
		return;
	if (FMath::Abs(Hit.Normal.Z) < 0.5f)
	{
		FVector ForwardVector = GetActorForwardVector();
		FVector SurfaceDirection = -Hit.Normal; 
		SurfaceDirection.Z = 0.f;
		SurfaceDirection.Normalize();
		float ForwardDot = FVector::DotProduct(ForwardVector, SurfaceDirection);
		if (ForwardDot > 0.7f)
		{
			// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Here %f"), Stats->GetMovementSpeed()));
			bShouldClimb = true;
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsAlive || UGameplayStatics::IsGamePaused(GetWorld()))
		return;

	FVector ToPlayer = PlayerCharacter->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.f;
	ToPlayer.Normalize();

	FVector CurrentVelocity = CapsuleComponent->GetPhysicsLinearVelocity();

	if (bShouldClimb)
	{
		CapsuleComponent->SetPhysicsLinearVelocity(FVector::UnitZ() * EnemyData->ClimbSpeed);
		bShouldClimb = false;
	}
	else if (CurrentVelocity.Size2D() < EnemyData->Speed)
	{
		CapsuleComponent->AddForce(EnemyData->MoveForce * ToPlayer);
	}

	// Rotation
	FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), PlayerCharacter->GetActorLocation());
	Mesh->SetRelativeRotation(FRotator(0, NewRotator.Yaw - 90.f, 0));

	// Relocate
	float Distance = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
	if (Distance > MaxDistance)
		AEnemyManager::GetInstance()->RelocateEnemy(this);
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
	
	TryDropChest();
	
	FreeItem();
	AEnemyManager::GetInstance()->EnemyDied(this);
}

void AEnemy::Initialize(UEnemyData* NewEnemyData)
{
	bIsAlive = true;
	EnemyData = NewEnemyData;
	Stats->Initialize(EnemyData->BaseStat);
	// Mesh->SetStaticMesh(EnemyData->SkeletalMesh);
	// DynMat = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, NewEnemyData->Material);
	// DynMat->SetScalarParameterValue(FName("TimeOffset"), FMath::RandRange(1, 100));
	// float temp;
	// DynMat->GetScalarParameterValue(FName("TimeOffset"), temp);
	SetActorScale3D(EnemyData->Scale);
	Speed = EnemyData->Speed;
	Damage = EnemyData->Damage;
	FVector Min, Max;
	Mesh->GetLocalBounds(Min, Max);
	FVector  Bounds = (Max - Min) * 0.5;
	CapsuleComponent->SetCapsuleSize(Bounds.X, Bounds.Z);
	PlayerCharacter->OnLoopRestarted.AddDynamic(this, &AEnemy::IncreaseLoopCounter);
	LoopCounter = 0;
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

void AEnemy::IncreaseLoopCounter()
{
	LoopCounter++;
}

