// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

class UCharacterStatData;
class AWeapnSystem;
/**
 * 
 */
UCLASS(Blueprintable)
class MUSICALVS_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> PassivesDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> BlessingsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> LevelsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AWeapnSystem> StartCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCharacterStatData> CharacterStatData;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void StartGame() const;
};
