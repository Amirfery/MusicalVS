// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class UHealthComponent;
class UEnemyData;
class UCapsuleComponent;

UCLASS()
class MUSICALVS_API AEnemy : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();
	virtual void Init_Implementation(APoolManager* PoolManager) override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Die();

	void Initialize(UEnemyData* NewEnemyData);

	void Freeze();
	void Unfreeze();

	virtual void SetFloatValues_Implementation(const TArray<float>& FloatValues) override;
	
protected:
	UFUNCTION()
	void TickEnemy(float DeltaTime);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostLoad() override;

public:
	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	float Damage;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> PlayerCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName XpPoolSystemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DamageTextPoolSystemName;
	
	UPROPERTY(Transient)
	TObjectPtr<UEnemyData> EnemyData;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DynMat;

	UPROPERTY(Transient)
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(Transient)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Transient)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(Transient)
	float MaxDistance;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive;
};
