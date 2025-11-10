// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::InitProjectile(const float ProjectileLifeSpan, const FVector& ProjectileVelocity, const float ProjectileDamage)
{
	// UKismetSystemLibrary::PrintString(GetWorld(), GetName(), true, false, FColor::Red, 1);
	LifeSpan = ProjectileLifeSpan;
	Velocity = ProjectileVelocity;
	Damage = ProjectileDamage;
	GetComponentByClass<UProjectileMovementComponent>()->SetVelocityInLocalSpace({1000, 0, 0});
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimer, FTimerDelegate::CreateLambda([this]
	{
		FreeItem();
	}), LifeSpan, false);
}

void AProjectile::FreeItem_Implementation()
{
	Super::FreeItem_Implementation();
	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimer);
}

