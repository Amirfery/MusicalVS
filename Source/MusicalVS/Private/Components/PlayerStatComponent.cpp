#include "Components/PlayerStatComponent.h"

#include "GameManager.h"
#include "DataAssets/StatData.h"

UPlayerStatComponent::UPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseStat = Cast<UGameManager>(GetWorld()->GetGameInstance())->CharacterStatData;

	CurrentHealth = BaseStat->MaxHealth;
	
	MaxHealthC = 0;
	RecoveryC = 0;
	ArmorC = 0;
	MovementSpeedC = 0;
	MightC = 0;
	AreaC = 0;
	ProjectileSpeedC = 0;
	DurationC = 0;
	LuckC = 0;
	GrowthC = 0;
	GreedC = 0;
	CurseC = 0;
	MagnetC = 0;
}

float UPlayerStatComponent::GetMaxHealth() const
{
	return BaseStat->MaxHealth * MaxHealthC;
}

float UPlayerStatComponent::GetRecovery() const
{
	return BaseStat->Recovery * RecoveryC;
}

float UPlayerStatComponent::GetArmor() const
{
	return BaseStat->Armor * ArmorC;
}

float UPlayerStatComponent::GetMovementSpeed() const
{
	return BaseStat->MovementSpeed * (1 + MovementSpeedC);
}

float UPlayerStatComponent::GetMight() const
{
	return BaseStat->Might * MightC;
}

float UPlayerStatComponent::GetArea() const
{
	return BaseStat->Area * AreaC;
}

float UPlayerStatComponent::GetProjectileSpeed() const
{
	return BaseStat->ProjectileSpeed * ProjectileSpeedC;
}

float UPlayerStatComponent::GetDuration() const
{
	return BaseStat->Duration * DurationC;
}

float UPlayerStatComponent::GetLuck() const
{
	return BaseStat->Luck * LuckC;
}

float UPlayerStatComponent::GetGrowth() const
{
	return BaseStat->Growth * GrowthC;
}

float UPlayerStatComponent::GetGreed() const
{
	return BaseStat->Greed * GreedC;
}

float UPlayerStatComponent::GetCurse() const
{
	return BaseStat->Curse * CurseC;
}

float UPlayerStatComponent::GetMagnet() const
{
	return BaseStat->Magnet * MagnetC;
}

void UPlayerStatComponent::DecreaseHealth(const float Amount)
{
	CurrentHealth -= Amount;
}

void UPlayerStatComponent::IncreaseHealth(const float Amount)
{
	CurrentHealth += Amount;
}






