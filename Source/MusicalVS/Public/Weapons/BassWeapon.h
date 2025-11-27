// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AoeWeapon.h"
#include "BassWeapon.generated.h"

UCLASS()
class MUSICALVS_API ABassWeapon : public AAoeWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	bool bIsActive = false;

	UPROPERTY(Transient)
	FTimerHandle AttackTimerHandle;

private:
	virtual void OnTimelineMarker(FString Name, int32 Position) override;

protected:
	virtual void BeginPlay() override;

public:
	ABassWeapon();

	virtual void PostInitializeComponents() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack_Implementation() override;

	virtual void Upgrade_Implementation() override;
	
};
