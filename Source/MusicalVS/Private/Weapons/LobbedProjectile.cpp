// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LobbedProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ALobbedProjectile::ALobbedProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbedProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbedProjectile::InitProjectile(const FVector& Target, const FProjectileData& ProjectileData)
{
	Damage = ProjectileData.Damage;
	auto Component = GetComponentByClass<UProjectileMovementComponent>();
	const float Vz = ProjectileData.Velocity * -GetWorld()->GetGravityZ() * Component->ProjectileGravityScale * 0.5f;
	const float Vx = (Target.X - GetActorLocation().X) / ProjectileData.Velocity;
	const float Vy = (Target.Y - GetActorLocation().Y) / ProjectileData.Velocity;
	Component->Velocity = FVector(Vx, Vy, Vz);

}

