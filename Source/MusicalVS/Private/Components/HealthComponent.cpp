#include "Components/HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::DecreaseHealth(const float Amount)
{
	if (Amount <= 0)
		return;

	const float PrevHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0, MaxHealth);
	const float HealthDelta = CurrentHealth - PrevHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, HealthDelta);
	if (CurrentHealth <= 0)
		GetOwner()->Destroy();
}

void UHealthComponent::IncreaseHealth(const float Amount)
{
	if (Amount <= 0)
		return;

	const float PrevHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	const float HealthDelta = CurrentHealth - PrevHealth;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, HealthDelta);
}
