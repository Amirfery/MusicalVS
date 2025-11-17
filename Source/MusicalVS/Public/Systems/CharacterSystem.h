// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterSystem.generated.h"

struct FWeaponToUpgrade;
class AWeapnSystem;
class APoolManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneXPAdded, float, Amount);

UCLASS()
class MUSICALVS_API ACharacterSystem : public ACharacter
{
	GENERATED_BODY()

private:
	static ACharacterSystem* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FName, TObjectPtr<AWeapnSystem>> Weapons;

	FTimerHandle AutoAttackTimer;

	UPROPERTY(BlueprintAssignable)
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable)
	FOneXPAdded OnXPAdded;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 XP;
	UPROPERTY(BlueprintReadWrite)
	int32 Level;
	UPROPERTY(BlueprintReadWrite)
	int32 NeededXpToLevelUp;

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
	void AddXP(int32 Amount);

	UFUNCTION(BlueprintCallable)
	TArray<FWeaponToUpgrade> GetWeaponUpgrades();

	UFUNCTION(BlueprintCallable)
	void SetPaused(bool Paused);

	UFUNCTION(BlueprintCallable)
	void UpgradeWeapon(FName Id);

private:
	UFUNCTION(BlueprintCallable)
	void Init();
};
