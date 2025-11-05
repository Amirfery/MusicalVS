// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AoeWeapon.h"

#include "Components/HealthComponent.h"
#include "DataAssets/AttackData.h"
#include "Systems/CharacterSystem.h"


// Sets default values
AAoeWeapon::AAoeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAoeWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!IsValid(WeaponData))
		return;
	Radius = WeaponData->BaseRange;
	Damage = WeaponData->BaseDamage;
	EffectTime = WeaponData->BaseEffectTime;
}

void AAoeWeapon::BeginPlay()
{
	Super::BeginPlay();
	FmodAudioComp->SetParameter(FName(TEXT("Drums Level")), 0);
}

void AAoeWeapon::OnEnemyEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacterSystem::StaticClass()))
		return;
	if (OtherActor && OtherActor != this)
	{
		EnemiesInRange.Add(OtherActor);
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), EnemiesInRange.Num());
}

void AAoeWeapon::OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		EnemiesInRange.Remove(OtherActor);
	}
}

// Called every frame
void AAoeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAoeWeapon::Upgrade_Implementation()
{
	Super::Upgrade_Implementation();
	FAttackLevelStruct LevelUp = WeaponData->LevelUps[Level];
	Damage = Damage + (Damage * LevelUp.DamageUp);
	EffectTime = EffectTime + (EffectTime * LevelUp.DamageUp);
	Radius =Radius + (Radius * LevelUp.RangeUp);
	Level += 1;
	// FmodAudioComp->ParameterCache.Add(FName(TEXT("Drums Level")), Level);
	FmodAudioComp->SetParameter(FName(TEXT("Drums Level")), Level);
}

void AAoeWeapon::Attack_Implementation()
{
	Super::Attack_Implementation();
	for (AActor* Enemy : EnemiesInRange)
	{
		if (!IsValid(Enemy))
			continue;
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Enemy->GetRootComponent()))
		{
			FVector Dir = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Dir.Z = 0.f;
			Dir.Normalize();
			Prim->AddImpulse(Dir * EffectTime, NAME_None, true);
		}
		if (UHealthComponent* HealthComponent = Enemy->GetComponentByClass<UHealthComponent>())
		{
			HealthComponent->DecreaseHealth(Damage);
		}
	}
}
