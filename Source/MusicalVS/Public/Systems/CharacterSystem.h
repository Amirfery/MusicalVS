// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterSystem.generated.h"

class AWeapnSystem;
class APoolManager;

UCLASS()
class MUSICALVS_API ACharacterSystem : public ACharacter
{
	GENERATED_BODY()

private:
	static ACharacterSystem* Instance;

	UPROPERTY(EditAnywhere)
	TArray<AWeapnSystem*> Weapons;

	FTimerHandle AutoAttackTimer;

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

private:
	UFUNCTION(BlueprintCallable)
	void Init();
};
