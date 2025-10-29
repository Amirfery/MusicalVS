// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/WeapnSystem.h"
#include "ProjectileWeapon.generated.h"

class APoolManager;

UCLASS()
class MUSICALVS_API AProjectileWeapon : public AWeapnSystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	APoolManager* ProjectilePoolManager;

	virtual void BeginPlay() override;

private:
	void SpawnProjectileAtEnemy(const AActor* TargetEnemy) const;

public:
	AProjectileWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack_Implementation() override;
};

