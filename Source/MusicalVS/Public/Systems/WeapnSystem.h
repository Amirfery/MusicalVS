// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "WeapnSystem.generated.h"

struct FAttackLevelStruct;
class UAttackData;
class ACharacterSystem;

UCLASS()
class MUSICALVS_API AWeapnSystem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	ACharacterSystem* Character;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UFMODAudioComponent* FmodAudioComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	float RotationSafeDegree = 30.0f;

	UPROPERTY(EditDefaultsOnly)
	float RotateSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float RotateRadius = 200.0f;

	UPROPERTY(Transient)
	float RotationTime;

	UPROPERTY(Transient)
	FVector PrevLocationInOrbit;

	UPROPERTY(Transient)
	FQuat RandomRotation;

	UPROPERTY(Transient)
	bool bShouldAttack;

public:
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttackData> AttackData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Id;

	bool bShouldSync;
	
public:
	// Sets default values for this actor's properties
	AWeapnSystem();

private:
	UFUNCTION()
	virtual void OnTimelineMarker(FString Name, int32 Position);

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Upgrade();

	UFUNCTION(BlueprintNativeEvent)
	void FirstMarkerAttack();

	UFUNCTION(BlueprintNativeEvent)
	void SecondMarkerAttack();
	
	UFUNCTION(BlueprintNativeEvent)
	void ThirdMarkerAttack();

	UFUNCTION(BlueprintNativeEvent)
	void FourthMarkerAttack();
	
	UFUNCTION(BlueprintNativeEvent)
	void Attack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartSolo();

	UFUNCTION(BlueprintCallable)
	float GetEventPercentage();

	void SetPaused(bool Paused);
	void SetEventPercentage(float Percentage);
	void UpdateRotationAroundCharacter(float DeltaTime);
};
