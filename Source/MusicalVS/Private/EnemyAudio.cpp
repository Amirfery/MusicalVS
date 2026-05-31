// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAudio.h"

#include "FMODAudioComponent.h"
#include "DataAssets/EnemyData.h"
#include "Systems/CharacterSystem.h"


void AEnemyAudio::StartPlaying()
{
	ChangeVolume(1.0);
	FModAudio->Play();
	ACharacterSystem::GetCharacterInstance()->OnLoopRestarted.RemoveDynamic(this, &AEnemyAudio::StartPlaying);
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

