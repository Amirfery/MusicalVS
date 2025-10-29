// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AoeWeapon.h"

#include "DataAssets/AttackData.h"
#include "Systems/CharacterSystem.h"


// Sets default values
AAoeWeapon::AAoeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAoeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Radius = WeaponData->BaseRange;
	Damage = WeaponData->BaseDamage;
	
	DetectionSphere->InitSphereRadius(Radius);
	DetectionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAoeWeapon::OnEnemyEnterRange);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AAoeWeapon::OnEnemyExitRange);
}

void AAoeWeapon::OnEnemyEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterSystem::StaticClass()))
		return;
	if (OtherActor && OtherActor != this)
	{
		EnemiesInRange.Add(OtherActor);
	}
}

void AAoeWeapon::OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		EnemiesInRange.Remove(OtherActor);
	}
}

// Called every frame
void AAoeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAoeWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
	for (AActor* Enemy : EnemiesInRange)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Enemy->GetRootComponent()))
		{
			FVector Dir = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Dir.Z = 0.f;
			Dir.Normalize();
			Prim->AddImpulse(Dir * Damage, NAME_None, true);
		}
	}
}