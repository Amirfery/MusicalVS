
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

class UCharacterStatData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSICALVS_API UPlayerStatComponent : public UActorComponent
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
	float LuckC;
	float GrowthC;
	float GreedC;
	float CurseC;
	float MagnetC;

protected:
	virtual void BeginPlay() override;

public:
	UPlayerStatComponent();

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLuck() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetGrowth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetGreed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurse() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMagnet() const;

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(const float Amount);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(const float Amount);
};
