// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Systems/WeapnSystem.h"
#include "AoeWeapon.generated.h"

UCLASS()
class MUSICALVS_API AAoeWeapon : public AWeapnSystem
{
	GENERATED_BODY()

private:
	FTimerHandle SlamTimerHandle;
	FTimerHandle RiseTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<FOverlapResult> EnemiesInRange;

	UPROPERTY(EditDefaultsOnly)
	float FallingTime;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	float EffectTime;

public:
	// Sets default values for this actor's properties
	AAoeWeapon();

private:
	UFUNCTION()
	void SlamDown();

	UFUNCTION()
	void OnLandedCallback();

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OnEnemyEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						   const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Upgrade_Implementation() override;

	virtual void Attack_Implementation() override;

	virtual void FirstMarkerAttack_Implementation() override;

	virtual void SecondMarkerAttack_Implementation() override;

	virtual void ThirdMarkerAttack_Implementation() override;

	virtual void FourthMarkerAttack_Implementation() override;

	virtual void PreStartSoloPhase_Implementation() override;

	virtual void StartSoloPhase_Implementation() override;
};
