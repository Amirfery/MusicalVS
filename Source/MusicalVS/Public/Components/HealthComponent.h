#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, float, Health, float, MaxHealth, float, HealthDelta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSICALVS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleInstanceOnly)
	float CurrentHealth;

public:
	UHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(const float Amount);

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(const float Amount);

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const {return CurrentHealth;}
};
