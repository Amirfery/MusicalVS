#include "Components/EnemyStatComponent.h"

#include "DataAssets/StatData.h"


UEnemyStatComponent::UEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyStatComponent::Initialize(UCharacterStatData* BaseStatTemp)
{
	BaseStat = BaseStatTemp;
	CurrentHealth = GetMaxHealth();
}

float UEnemyStatComponent::GetMaxHealth() const
{
	return BaseStat->MaxHealth * (1 + MaxHealthC);
}

float UEnemyStatComponent::GetRecovery() const
{
	return BaseStat->Recovery * (1 + RecoveryC);
}

float UEnemyStatComponent::GetArmor() const
{
	return BaseStat->Armor * (1 + ArmorC);
}

float UEnemyStatComponent::GetMovementSpeed() const
{
	return BaseStat->MovementSpeed * (1 + MovementSpeedC);
}

float UEnemyStatComponent::GetMight() const
{
	return BaseStat->Might * (1 + MightC);
}

float UEnemyStatComponent::GetArea() const
{
	return BaseStat->Area * (1 + AreaC);
}

float UEnemyStatComponent::GetProjectileSpeed() const
{
	return BaseStat->ProjectileSpeed * (1 + ProjectileSpeedC);
}

float UEnemyStatComponent::GetDuration() const
{
	return BaseStat->Duration * (1 + DurationC);
}

void UEnemyStatComponent::DecreaseHealth(const float Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0)
		OnEnemyDied.Broadcast();
}

void UEnemyStatComponent::IncreaseHealth(const float Amount)
{
	CurrentHealth += Amount;
	if (CurrentHealth >= GetMaxHealth())
		CurrentHealth = GetMaxHealth();
}


