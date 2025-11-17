#pragma once

#include "GenericStructs.generated.h"

class UAttackData;
class AWeapnSystem;

USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeapnSystem> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttackData> AttackData;
};

USTRUCT(BlueprintType)
struct FWeaponToUpgrade
{
	GENERATED_BODY()

	FWeaponToUpgrade() : Id(TEXT("")), DisplayName(TEXT("")), Description(FText::GetEmpty()), Icon(nullptr) {}

	FWeaponToUpgrade(FName IdTemp, FName DisplayNameTemp, FText DescriptionTemp, TObjectPtr<UTexture2D> IconTemp)
	{
		Id = IdTemp;
		DisplayName = DisplayNameTemp;
		Description = DescriptionTemp;
		Icon = IconTemp;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
};
