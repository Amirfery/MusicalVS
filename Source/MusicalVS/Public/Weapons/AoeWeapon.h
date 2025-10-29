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
	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere)
	TSet<AActor*> EnemiesInRange;

	UPROPERTY(EditDefaultsOnly)
	float Radius;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

public:
	// Sets default values for this actor's properties
	AAoeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnemyEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						   const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack_Implementation() override;
};
