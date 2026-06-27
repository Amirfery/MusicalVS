// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CharacterSystem.h"

#include "EnemyAudio.h"
#include "EnemyManager.h"
#include "GameManager.h"
#include "Animation/CharacterAnimInstance.h"
#include "Components/PlayerStatComponent.h"
#include "DataAssets/AttackData.h"
#include "DataAssets/EnemyData.h"
#include "DataAssets/PassiveData.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Infrastructure/GenericStructs.h"
#include "Interfaces/Interactable.h"
#include "Systems/BlessingSystem.h"
#include "Systems/PassiveSystem.h"
#include "Systems/WeapnSystem.h"

class UGameManager;
ACharacterSystem* ACharacterSystem::Instance = nullptr;

// Sets default values
ACharacterSystem::ACharacterSystem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Instance = this;
	CurrentInteractable = nullptr;

	Stats = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("Stats"));
}

// Called when the game starts or when spawned
void ACharacterSystem::BeginPlay()
{
	Super::BeginPlay();
	Level = 0;
	NeededXpToLevelUp = 10;
	SoulNote = 0;
	XP = 0;
	PrevTickEventPercentage = 100.0f;
	bIsRising = false;
	CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, this, &ACharacterSystem::FindAndAttackNearestEnemy, 0.5f, true);
	// GetWorldTimerManager().SetTimer(AutoAttackTimer, [this, ]&ACharacterSystem::AoeAttack, 0.5f, true);
}

// Called every frame
void ACharacterSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = Stats->GetMovementSpeed();
	const float CurrentTickEventPercentage = MainWeapon->GetEventPercentage();
	// UKismetSystemLibrary::PrintString(
	// 	GetWorld(),
	// 	FString::Printf(TEXT("Percentage: %.2f"), CurrentTickEventPercentage),
	// 	true,
	// 	true,   // Print to log
	// 	FLinearColor::Green,
	// 	2.0f    // Duration
	// );
	if (CurrentTickEventPercentage < PrevTickEventPercentage)
	{
		OnLoopRestarted.Broadcast();
	}
	PrevTickEventPercentage = CurrentTickEventPercentage;
	if (bIsRising)
	{
		RisingElapsedTime += DeltaTime;
		FVector CurrentLocation = GetActorLocation();
		SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y,
		                         (RisingCurve->GetFloatValue(RisingElapsedTime / RisingTime) * RisingHeight) +
		                         StartingHeight));
	}
	if (bIsFalling)
	{
		AddActorWorldOffset(FVector(0, 0, -1.f * FallingSpeed * DeltaTime));
		if (GetActorLocation().Z < FallingDestination.Z + 100)
		{
			OnPlayerLanded.Broadcast();
		}
	}

	PerformInteractionTrace();
}

// Called to bind functionality to input
void ACharacterSystem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterSystem::AddWeapon(AWeapnSystem* Weapon)
{
	if (Weapons.IsEmpty())
	{
		MainWeapon = Weapon;
		MainWeapon->SetMainWeapon();
	}
		

	Weapons.Add(Weapon->Id, Weapon);
	SynchronizeAudio();
}

void ACharacterSystem::AddBlessing(ABlessingSystem* Blessing)
{
	Blessings.Add(Blessing->Id, Blessing);
	SynchronizeAudio();
}

void ACharacterSystem::AddNewPhaseEnemyAudios(TArray<FEnemySpawnInfo> NewPhaseEnemies)
{
	TMap<FName, TObjectPtr<AEnemyAudio>> NewEnemyAudios;
	for (auto& Element : NewPhaseEnemies)
	{
		UEnemyData* EnemyData = Element.EnemyDataAsset;
		if (EnemyData->SoundEvent != nullptr && !ActiveEnemyAudios.Contains(EnemyData->DisplayName))
		{
			TObjectPtr<AEnemyAudio> NewEnemyAudio = GetWorld()->SpawnActor<AEnemyAudio>();
			NewEnemyAudio->Initialize(EnemyData);
			NewEnemyAudio->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			NewEnemyAudios.Add(EnemyData->DisplayName, NewEnemyAudio);
		}
		else
		{
			NewEnemyAudios.Add(EnemyData->DisplayName, ActiveEnemyAudios.Find(EnemyData->DisplayName)->Get());
		}
	}
	
	for (auto& AudioPair : ActiveEnemyAudios)
	{
		if (!NewEnemyAudios.Contains(AudioPair.Key))
		{
			AudioPair.Value->Stop();
		}
	}
	ActiveEnemyAudios = NewEnemyAudios;
}

void ACharacterSystem::AddSoulNote(const int32 Amount)
{
	SoulNote += Amount;
	OnSoulNoteAdded.Broadcast(SoulNote);
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

void ACharacterSystem::PerformUpgraderRitual()
{
	InitialSoulNoteAmount = SoulNote;
	GetWorld()->GetTimerManager().SetTimer(SoulNoteToXpTimerHandle, FTimerDelegate::CreateLambda([this]
	{
		if (SoulNote > 0)
		{
			const float Progress = 1.f - (float)SoulNote / InitialSoulNoteAmount;

			const int32 Amount = FMath::Lerp(1, SoulNote, Progress);
			AddXP(Amount);
			AddSoulNote(-1 * Amount);
		} 
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(SoulNoteToXpTimerHandle);
		}
	}), 0.1f, true);
}

TArray<FName> ACharacterSystem::GetWeaponUpgrades()
{
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());

	UDataTable* Table = GM->WeaponsDataTable;

	if (!Table)
		return TArray<FName>();

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
	return ValidRows;
}

TArray<FName> ACharacterSystem::GetPassiveUpgrades()
{
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());

	UDataTable* Table = GM->PassivesDataTable;

	if (!Table)
		return TArray<FName>();

	TArray<FName> AllRows = Table->GetRowNames();
	TArray<FName> ValidRows;
	for (FName Row : AllRows)
	{
		if (Passives.Contains(Row))
		{
			if (Passives[Row]->PassiveData->LevelUps.Num() > Passives[Row]->Level)
				ValidRows.Add(Row);
		}
		else if (Passives.Num() < 10)
			ValidRows.Add(Row);
	}

	return ValidRows;
}

TArray<FWeaponToUpgrade> ACharacterSystem::GetUpgrades(const int Count)
{
	TArray<FName> AllUpgrades;
	AllUpgrades.Append(GetWeaponUpgrades());
	AllUpgrades.Append(GetPassiveUpgrades());

	if (AllUpgrades.IsEmpty())
		return {};

	for (int32 i = AllUpgrades.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::RandRange(0, i);
		AllUpgrades.Swap(i, j);
	}

	const int32 FinalCount = FMath::Min(Count, AllUpgrades.Num());

	UGameManager* GM = Cast<UGameManager>(GetGameInstance());
	check(GM);

	TArray<FWeaponToUpgrade> Upgrades;

	for (int32 i = 0; i < FinalCount; ++i)
	{
		const FName RowName = AllUpgrades[i];

		if (const FWeapon* WeaponRow = GM->WeaponsDataTable->FindRow<FWeapon>(RowName, TEXT("Weapon Lookup")))
		{
			Upgrades.Emplace(
				RowName,
				WeaponRow->DisplayName,
				WeaponRow->Description,
				WeaponRow->Icon
			);
			continue;
		}

		if (const FPassive* PassiveRow = GM->PassivesDataTable->FindRow<FPassive>(RowName, TEXT("Passive Lookup")))
		{
			Upgrades.Emplace(
				RowName,
				PassiveRow->DisplayName,
				PassiveRow->Description,
				PassiveRow->Icon
			);
		}
	}

	if (!Upgrades.IsEmpty())
	{
		SetLowpass(true);
	}

	return Upgrades;
}


TArray<FWeaponToUpgrade> ACharacterSystem::GetBlessingUpgrades()
{
	UGameManager* GM = Cast<UGameManager>(GetGameInstance());

	UDataTable* Table = GM->BlessingsDataTable;

	if (!Table)
		return TArray<FWeaponToUpgrade>();

	TArray<FName> AllRows = Table->GetRowNames();
	TArray<FName> ValidRows;
	for (FName Row : AllRows)
	{
		if (!Blessings.Contains(Row))
		{
			ValidRows.Add(Row);
		}
	}
	TArray<FName> RandomRows;

	if (ValidRows.Num() > 3)
	{
		ValidRows.Sort([](const FName& A, const FName& B)
		{
			return FMath::RandRange(0, 1) == 0;
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
		FBlessingWeaponData* RowHandle = Table->FindRow<FBlessingWeaponData>(
			RowName, TEXT("Get a row from blessings data table."));
		Upgrades.Add(FWeaponToUpgrade(RowName, RowHandle->DisplayName, RowHandle->Description, RowHandle->Icon));
	}

	if (Upgrades.Num() > 0)
		SetLowpass(true);

	return Upgrades;
}

void ACharacterSystem::SetPaused(bool Paused)
{
	bIsGamePaused = Paused;
	for (const TPair<FName, TObjectPtr<AWeapnSystem>>& Weapon : Weapons)
	{
		Weapon.Value->SetPaused(Paused);
	}
	for (const TPair<FName, TObjectPtr<ABlessingSystem>>& Blessing : Blessings)
	{
		Blessing.Value->SetPaused(Paused);
	}
	for (const TPair<FName, TObjectPtr<AEnemyAudio>>& EnemyAudio : ActiveEnemyAudios)
	{
		EnemyAudio.Value->SetPaused(Paused);
	}
}

void ACharacterSystem::SetLowpass(bool isLowPass)
{
	float lowPass = isLowPass ? 1.0f : 0.0f;
	for (const TPair<FName, TObjectPtr<AWeapnSystem>>& Weapon : Weapons)
	{
		Weapon.Value->SetLowPass(lowPass);
	}
	for (const TPair<FName, TObjectPtr<ABlessingSystem>>& Blessing : Blessings)
	{
		Blessing.Value->SetLowPass(lowPass);
	}
	for (const TPair<FName, TObjectPtr<AEnemyAudio>>& EnemyAudio : ActiveEnemyAudios)
	{
		EnemyAudio.Value->SetLowPass(lowPass);
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
	//Actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	AddWeapon(Cast<AWeapnSystem>(Actor));
}

void ACharacterSystem::StartSolo()
{
	CurrentFadeTime = 0.f;
	RisingElapsedTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(
		VolumeFadeTimer,
		this,
		&ACharacterSystem::VolumeFadeout,
		0.1f,
		true
	);

	for (auto Element : Weapons)
	{
		Element.Value->ToggleShouldAttack();
	}

	MainWeapon->SoloAttack();
}

void ACharacterSystem::StopSolo()
{
	for (auto Element : Weapons)
	{
		Element.Value->ToggleShouldAttack();
		Element.Value->SetEventPercentage(0.f);
		Element.Value->ChangeVolume(1.f);
	}
}

void ACharacterSystem::Init()
{
}

void ACharacterSystem::VolumeFadeout()
{
	CurrentFadeTime += 0.1f;
	if (CurrentFadeTime >= MainWeapon->AttackData->VolumeReductionTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(VolumeFadeTimer);
	}
	for (auto Element : Weapons)
	{
		Element.Value->ChangeVolume(-0.1f / MainWeapon->AttackData->VolumeReductionTime);
	}
}

void ACharacterSystem::OnInteractPressed()
{
	if (CurrentInteractable.IsValid() && CurrentInteractable->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(CurrentInteractable.Get(), this);
	}
}

void ACharacterSystem::PerformInteractionTrace()
{
	UE_LOG(LogTemp, Display, TEXT("Start interaction"));
	TArray<FOverlapResult> OverlapResults;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);

	GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel4, Sphere);

	if (!OverlapResults.IsEmpty())
	{
		AActor* SelectedActor = nullptr;
		float MinDistance = InteractionRadius;
		float MaxProduct = FMath::Cos(FMath::DegreesToRadians(InteractionLookAngleThresholdDegree));
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && HitActor->Implements<UInteractable>())
			{
				auto TargetVector = HitActor->GetActorLocation() - GetActorLocation();
				float Distance = TargetVector.Length();
				float Product = GetActorForwardVector().Dot(TargetVector.GetSafeNormal());
				if (Product > MaxProduct || (Distance < MinDistance && Product == MaxProduct))
				{
					MinDistance = Distance;
					MaxProduct = Product;
					SelectedActor = HitActor;
				}
			}
		}

		if (SelectedActor)
		{
			SelectCandidateInteractable(SelectedActor);
		}
		else
		{
			UnselectCandidateInteractable();
		}
	}
}

void ACharacterSystem::SelectCandidateInteractable(AActor* Selected)
{
	if (CurrentInteractable != Selected)
	{
		UnselectCandidateInteractable();
		CurrentInteractable = Selected;
		if (CurrentInteractable.IsValid() && CurrentInteractable->Implements<UInteractable>())
		{
			IInteractable::Execute_SelectCandidateInteractable(CurrentInteractable.Get());
		}
	}
}

void ACharacterSystem::UnselectCandidateInteractable()
{
	if (CurrentInteractable.IsValid() && CurrentInteractable->Implements<UInteractable>())
	{
		IInteractable::Execute_UnSelectCandidateInteractable(CurrentInteractable.Get());
	}
	CurrentInteractable.Reset();
}

void ACharacterSystem::SynchronizeAudio()
{
	for (const auto Temp : Weapons)
	{
		Temp.Value->SetEventPercentage(0);
		Temp.Value->SetPaused(bIsGamePaused);
	}
	for (const auto Temp : Weapons)
	{
		Temp.Value->SetEventPercentage(0);
		Temp.Value->SetPaused(bIsGamePaused);
	}
	for (const auto Temp : ActiveEnemyAudios)
	{
		Temp.Value->SetEventPercentage(0);
		Temp.Value->SetPaused(bIsGamePaused);
	}
}
