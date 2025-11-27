// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "Projectile.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};
UCLASS()
class MUSICALVS_API AProjectile : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(BlueprintReadOnly)
	float LifeSpan;

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	float Damage;
	
private:
	FTimerHandle LifeSpanTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InitProjectile(const FVector& Target, const FProjectileData& ProjectileData);

	virtual void FreeItem_Implementation() override;
};
