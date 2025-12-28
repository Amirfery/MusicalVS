// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AoeWeapon.h"

#include "Components/HealthComponent.h"
#include "DataAssets/AttackData.h"
#include "Engine/OverlapResult.h"
#include "Interfaces/Damageable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/CharacterSystem.h"

#define ECC_Target ECC_GameTraceChannel2

// Sets default values
AAoeWeapon::AAoeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAoeWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!IsValid(AttackData))
		return;
	Radius = AttackData->BaseRange;
	Damage = AttackData->BaseDamage;
	EffectTime = AttackData->BaseEffectTime;
}

void AAoeWeapon::BeginPlay()
{
	Super::BeginPlay();
	FmodAudioComp->SetParameter(FName(TEXT("Level")), 0);
}

void AAoeWeapon::OnEnemyEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterSystem::StaticClass()))
		return;
	// if (OtherActor && OtherActor != this)
	// {
	// 	EnemiesInRange.Add(OtherActor);
	// }
	// UE_LOG(LogTemp, Warning, TEXT("%d"), EnemiesInRange.Num());
}

void AAoeWeapon::OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if (OtherActor && OtherActor != this)
	// {
	// 	EnemiesInRange.Remove(OtherActor);
	// }
}

// Called every frame
void AAoeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAoeWeapon::Upgrade_Implementation()
{
	Super::Upgrade_Implementation();
	FAttackLevelStruct LevelUp = AttackData->LevelUps[Level];
	Damage = Damage + (Damage * LevelUp.DamageUp);
	EffectTime = EffectTime + (EffectTime * LevelUp.DamageUp);
	Radius =Radius + (Radius * LevelUp.RangeUp);
	Level += 1;
	// FmodAudioComp->ParameterCache.Add(FName(TEXT("Drums Level")), Level);
	FmodAudioComp->SetParameter(FName(TEXT("Level")), Level);
}

void AAoeWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	const bool bHasHit = GetWorld()->OverlapMultiByChannel( Overlaps, GetActorLocation(), FQuat::Identity, ECC_Target, Sphere );

	EnemiesInRange = Overlaps;
	for (auto EnemyOverlap : EnemiesInRange)
	{
		AActor* Enemy = EnemyOverlap.GetActor();
		if (!IsValid(Enemy))
			continue;
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Enemy->GetRootComponent()))
		{
			FVector Dir = (Enemy->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
			Dir.Z = 0.f;
			Dir.Normalize();
			Prim->AddImpulse(Dir * EffectTime, NAME_None, true);
		}
		if (Enemy->Implements<UDamageable>())
			IDamageable::Execute_TakeDamage(Enemy, Damage);
	}
}

void AAoeWeapon::FirstMarkerAttack_Implementation()
{
	Super::FirstMarkerAttack_Implementation();
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
			IDamageable::Execute_TakeDamage(Enemy, Damage / 2.f);
	}
}

void AAoeWeapon::SecondMarkerAttack_Implementation()
{
	Super::SecondMarkerAttack_Implementation();
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

void AAoeWeapon::ThirdMarkerAttack_Implementation()
{
	Super::ThirdMarkerAttack_Implementation();
	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	const bool bHasHit = GetWorld()->OverlapMultiByChannel( Overlaps, GetActorLocation(), FQuat::Identity, ECC_Target, Sphere );

	EnemiesInRange = Overlaps;
	for (auto EnemyOverlap : EnemiesInRange)
	{
		AActor* Enemy = EnemyOverlap.GetActor();
		if (!IsValid(Enemy))
			continue;
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Enemy->GetRootComponent()))
		{
			FVector Dir = (Enemy->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
			Dir.Z = 0.f;
			Dir.Normalize();
			Prim->AddImpulse(Dir * EffectTime, NAME_None, true);
		}
		if (Enemy->Implements<UDamageable>())
			IDamageable::Execute_TakeDamage(Enemy, Damage);
	}
}
