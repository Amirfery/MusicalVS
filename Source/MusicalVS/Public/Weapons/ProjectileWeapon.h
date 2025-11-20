// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/WeapnSystem.h"
#include "ProjectileWeapon.generated.h"

class APoolManager;

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Random		UMETA(DisplayName="Random"),
	Nearest		UMETA(DisplayName="Nearest")
};

UCLASS()
class MUSICALVS_API AProjectileWeapon : public AWeapnSystem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	float SearchRadius;

	float Damage;

	float EffectTime;

	UPROPERTY(EditDefaultsOnly)
	ETargetType TargetType;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName PoolId;

	virtual void BeginPlay() override;


private:
	void SpawnProjectileAtEnemy(const AActor* TargetEnemy) const;

public:
	AProjectileWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Upgrade_Implementation() override;
	
	virtual void Attack_Implementation() override;
};

