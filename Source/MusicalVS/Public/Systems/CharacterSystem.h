// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Infrastructure/GenericStructs.h"
#include "CharacterSystem.generated.h"

class APassiveSystem;
class UPlayerStatComponent;
struct FWeaponToUpgrade;
class AWeapnSystem;
class APoolManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLanded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoopRestarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneXPAdded, float, Amount);

UCLASS()
class MUSICALVS_API ACharacterSystem : public ACharacter
{
	GENERATED_BODY()

private:
	static ACharacterSystem* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FName, TObjectPtr<AWeapnSystem>> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FName, TObjectPtr<ABlessingSystem>> Blessings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FName, TObjectPtr<APassiveSystem>> Passives;

	FTimerHandle AutoAttackTimer;
	FTimerHandle VolumeFadeTimer;

	float CurrentFadeTime;

	UPROPERTY(BlueprintAssignable)
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable)
	FOneXPAdded OnXPAdded;

	UPROPERTY(Transient)
	bool bIsGamePaused;

	UPROPERTY(Transient)
	float PrevTickEventPercentage;

	UPROPERTY(Transient)
	float RisingElapsedTime;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 XP;
	UPROPERTY(BlueprintReadWrite)
	int32 Level;
	UPROPERTY(BlueprintReadWrite)
	int32 NeededXpToLevelUp;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AWeapnSystem> MainWeapon;

	UPROPERTY(BlueprintAssignable)
	FLoopRestarted OnLoopRestarted;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerLanded OnPlayerLanded;

	UPROPERTY(BluePrintReadOnly)
	TObjectPtr<UPlayerStatComponent> Stats;

	UPROPERTY(Transient)
	bool bIsRising;
	UPROPERTY(EditDefaultsOnly)
	float RisingHeight;
	UPROPERTY(Transient)
	float StartingHeight;
	UPROPERTY(EditDefaultsOnly)
	float RisingTime;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> RisingCurve;

	UPROPERTY(Transient)
	bool bIsFalling;
	UPROPERTY(Transient)
	FVector FallingDestination;
	UPROPERTY(Transient)
	float FallingSpeed;

public:
	// Sets default values for this character's properties
	ACharacterSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ACharacterSystem* GetCharacterInstance() {return Instance;}

	UFUNCTION(BlueprintCallable)
	void AddWeapon(AWeapnSystem* Weapon);

	UFUNCTION(BlueprintCallable)
	void AddBlessing(ABlessingSystem* Blessing);

	UFUNCTION(BlueprintCallable)
	void AddXP(int32 Amount);

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetWeaponUpgrades();

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetPassiveUpgrades();

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponToUpgrade> GetUpgrades(const int Count = 3);

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponToUpgrade> GetBlessingUpgrades();

	UFUNCTION(BlueprintCallable)
	void SetPaused(bool Paused);

	UFUNCTION(BlueprintCallable)
	void UpgradeWeapon(FName Id);

	UFUNCTION(BlueprintCallable)
	void SetStartWeapon();

	UFUNCTION(BlueprintCallable)
	void StartSolo();

	UFUNCTION()
	void StopSolo();

private:
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void VolumeFadeout();
	
};
