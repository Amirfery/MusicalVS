// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"

class UCharacterStatData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDied);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSICALVS_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TObjectPtr<UCharacterStatData> BaseStat;

public:
	float CurrentHealth;
	
	float MaxHealthC;
	float RecoveryC;
	float ArmorC;
	float MovementSpeedC;
	float MightC;
	float AreaC;
	float ProjectileSpeedC;
	float DurationC;

	UPROPERTY(BlueprintAssignable)
	FEnemyDied OnEnemyDied;

protected:
	virtual void BeginPlay() override;

public:
	UEnemyStatComponent();

	void Initialize(UCharacterStatData* BaseStatTemp);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRecovery() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMight() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetArea() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetProjectileSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDuration() const;

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float Amount);
};
