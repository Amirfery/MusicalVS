// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAudio.h"

#include "Enemy.h"
#include "EnemyManager.h"
#include "FMODAudioComponent.h"
#include "DataAssets/EnemyData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/CharacterSystem.h"


void AEnemyAudio::StartPlaying()
{
	FModAudio->Play();
	ACharacterSystem::GetCharacterInstance()->OnLoopRestarted.RemoveDynamic(this, &AEnemyAudio::StartPlaying);
}

void AEnemyAudio::StopPlaying()
{
	FModAudio->Stop();
	ACharacterSystem::GetCharacterInstance()->OnLoopRestarted.RemoveDynamic(this, &AEnemyAudio::StartPlaying);
	Destroy();
}

void AEnemyAudio::UpdateVolume()
{
	TSet<TObjectPtr<AEnemy>> Enemies = EnemyManager->GetAliveEnemies(EnemyData->DisplayName);
	float MinDistance = TNumericLimits<float>::Max();
	FVector MinToEnemy = FVector::ZeroVector;
	FVector CharacterLocation = Character->GetActorLocation();
	for (TObjectPtr<AEnemy> Enemy : Enemies)
	{
		FVector ToEnemy = Enemy->GetActorLocation() - CharacterLocation;
		float Distance = ToEnemy.SizeSquared();
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			MinToEnemy = ToEnemy;
		}
	}
	MinDistance = FMath::Clamp(FMath::Sqrt(MinDistance), Character->MinHearingDistance, Character->MaxHearingDistance);
	float Volume = (Character->MaxHearingDistance - MinDistance) / (Character->MaxHearingDistance - Character->MinHearingDistance);
	ChangeVolume(Volume);
	SetActorLocation(CharacterLocation + MinToEnemy.GetSafeNormal() * FMath::Min(Character->MinHearingDistance, MinDistance));
}

// Sets default values
AEnemyAudio::AEnemyAudio()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FModAudio = CreateDefaultSubobject<UFMODAudioComponent>(FName("FmodAudioComponent"));
	FModAudio->SetupAttachment(RootComponent);
	FModAudio->bAutoActivate = false;
	FModAudio->bEnableTimelineCallbacks = true;
}

// Called when the game starts or when spawned
void AEnemyAudio::BeginPlay()
{
	Super::BeginPlay();
	EnemyManager = AEnemyManager::GetInstance();
	Character = ACharacterSystem::GetCharacterInstance();
}

// Called every frame
void AEnemyAudio::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAudio::Initialize(UEnemyData* NewEnemyData)
{
	EnemyData = NewEnemyData;
	FModAudio->SetEvent(EnemyData->SoundEvent);
	ACharacterSystem::GetCharacterInstance()->OnLoopRestarted.AddDynamic(this, &AEnemyAudio::StartPlaying);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyAudio::UpdateVolume, EnemyManager->UpdateVolumeInterval, true);
}

void AEnemyAudio::SetPaused(const bool Paused)
{
	FModAudio->SetPaused(Paused);
}

void AEnemyAudio::SetEventPercentage(float Percentage)
{
	FModAudio->SetTimelinePosition(FModAudio->GetLength() * Percentage);
}

void AEnemyAudio::ChangeVolume(const float Volume)
{
	CurrentVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
	FModAudio->SetParameter(FName("Volume"), CurrentVolume);
}

void AEnemyAudio::Stop()
{
	ACharacterSystem::GetCharacterInstance()->OnLoopRestarted.AddDynamic(this, &AEnemyAudio::StopPlaying);
}

