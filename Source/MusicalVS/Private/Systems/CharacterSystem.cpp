// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSystem.h"
#include "GameManager.h"
#include "LevelManager.h"
#include "DataAssets/AttackData.h"
#include "GameFramework/Character.h"
#include "Infrastructure/GenericStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/WeapnSystem.h"

class UGameManager;
ACharacterSystem* ACharacterSystem::Instance = nullptr;

// Sets default values
ACharacterSystem::ACharacterSystem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Instance = this;
}

// Called when the game starts or when spawned
void ACharacterSystem::BeginPlay()
{
	Super::BeginPlay();
	Level = 0;
	NeededXpToLevelUp = 10;
	XP = 0;
	PrevTickEventPercentage = 0.0f;
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, this, &ACharacterSystem::FindAndAttackNearestEnemy, 0.5f, true);
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, [this, ]&ACharacterSystem::AoeAttack, 0.5f, true);
}

// Called every frame
void ACharacterSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float CurrentTickEventPercentage = MainWeapon->GetEventPercentage();
	UKismetSystemLibrary::PrintString(
		GetWorld(),
		FString::Printf(TEXT("Percentage: %.2f"), CurrentTickEventPercentage),
		true,
		true,   // Print to log
		FLinearColor::Green,
		2.0f    // Duration
	);
	if (CurrentTickEventPercentage < PrevTickEventPercentage)
	{
		OnLoopRestarted.Broadcast();
	}
	PrevTickEventPercentage = CurrentTickEventPercentage;
}

// Called to bind functionality to input
void ACharacterSystem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterSystem::AddWeapon(AWeapnSystem* Weapon)
{
	if (Weapons.IsEmpty())
		MainWeapon = Weapon;
	
	Weapons.Add(Weapon->Id, Weapon);
	for (auto Temp : Weapons)
	{
		Temp.Value->SetEventPercentage(0);
		Temp.Value->SetPaused(bIsGamePaused);
	}
}

void ACharacterSystem::AddXP(int32 Amount)
{
	XP += Amount;
	const float XpProgress = XP / NeededXpToLevelUp;
	OnXPAdded.Broadcast(XpProgress);
	while (XP >= NeededXpToLevelUp)
	{
		XP -= NeededXpToLevelUp;
		Level++;
		// for (AWeapnSystem* Weapon : Weapons)
		// {
		// 	Weapon->Upgrade_Implementation();
		// }
		OnLevelUp.Broadcast();
		if (Level < 21)
		{
			NeededXpToLevelUp = Level * 10;
		}
		else if (Level > 20 && Level < 41)
		{
			NeededXpToLevelUp = 600 + Level * 13;
		}
		else if (Level > 40)
		{
			NeededXpToLevelUp = 2400 + Level * 16;
		}
	}
}

TArray<FWeaponToUpgrade> ACharacterSystem::GetWeaponUpgrades()
{
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());

	UDataTable* Table = GM->WeaponsDataTable;
	
	if (!Table)
		return TArray<FWeaponToUpgrade>();
	
	TArray<FName> AllRows = Table->GetRowNames();
	TArray<FName> ValidRows;
	for (FName Row : AllRows)
	{
		if (Weapons.Contains(Row))
		{
			if (Weapons[Row]->AttackData->LevelUps.Num() > Weapons[Row]->Level)
				ValidRows.Add(Row);
		}
		else if (Weapons.Num() < 10)
			ValidRows.Add(Row);
	}
	TArray<FName> RandomRows;

	if (ValidRows.Num() > 3)
	{
		ValidRows.Sort([](const FName& A, const FName& B)
		{
			return FMath::RandRange(0,1) == 0;
		});
		RandomRows.Append(ValidRows.GetData(), 3);
	}
	else
	{
		RandomRows = ValidRows;
	}

	TArray<FWeaponToUpgrade> Upgrades;
	for (FName RowName : RandomRows)
	{
		FWeapon* RowHandle = Table->FindRow<FWeapon>(RowName, TEXT("Get a row from weapon data table."));
		Upgrades.Add(FWeaponToUpgrade(RowName, RowHandle->DisplayName, RowHandle->Description, RowHandle->Icon));
	}
	
	if (Upgrades.Num() > 0)
		SetPaused(true);

	return Upgrades;
}

void ACharacterSystem::SetPaused(bool Paused)
{
	bIsGamePaused = Paused;
	for (const TPair<FName, TObjectPtr<AWeapnSystem>>& Weapon : Weapons)
	{
		Weapon.Value->SetPaused(Paused);
	}
}

void ACharacterSystem::UpgradeWeapon(FName Id)
{
	if (Weapons.Contains(Id))
	{
		Weapons[Id]->Upgrade();
	}
}

void ACharacterSystem::SetStartWeapon()
{
	TObjectPtr<AActor> Actor = GetWorld()->SpawnActor(Cast<UGameManager>(GetGameInstance())->StartCharacter);
	Actor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	AddWeapon(static_cast<AWeapnSystem*>(Actor));
}

void ACharacterSystem::Init()
{
}
