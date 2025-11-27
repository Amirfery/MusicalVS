
#include "Weapons/BassWeapon.h"
#include "DataAssets/AttackData.h"
#include "Engine/OverlapResult.h"
#include "Interfaces/Damageable.h"

#define ECC_Target ECC_GameTraceChannel2

ABassWeapon::ABassWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABassWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABassWeapon::OnTimelineMarker(FString Name, int32 Position)
{
	FString Marker = AttackData->AttackMarkers[0].ToString() + " " + FString::FromInt(Level);
	bIsActive = (!bIsActive);
	if (bIsActive)
	{
		GetWorldTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&ABassWeapon::Attack_Implementation,
			EffectTime,
			true
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ABassWeapon::BeginPlay()
{
	Super::BeginPlay();
	FmodAudioComp->SetParameter(FName(TEXT("Base Level")), 0);
}

void ABassWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsActive)
		Attack_Implementation();
}

void ABassWeapon::Attack_Implementation()
{
	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	
	const bool bHasHit = GetWorld()->OverlapMultiByChannel( Overlaps, GetActorLocation(), FQuat::Identity, ECC_Target, Sphere );
	
	EnemiesInRange = Overlaps;
	for (auto EnemyOverlap : EnemiesInRange)
	{
		AActor* Enemy = EnemyOverlap.GetActor();
		if (!IsValid(Enemy))
			continue;
		if (Enemy->Implements<UDamageable>())
			IDamageable::Execute_TakeDamage(Enemy, Damage);
	}
}

void ABassWeapon::Upgrade_Implementation()
{
	FAttackLevelStruct LevelUp = AttackData->LevelUps[Level];
	Damage = Damage + (Damage * LevelUp.DamageUp);
	EffectTime = EffectTime + (EffectTime * LevelUp.DamageUp);
	Radius =Radius + (Radius * LevelUp.RangeUp);
	Level += 1;
	FmodAudioComp->SetParameter(FName(TEXT("Bass Level")), Level);
}

