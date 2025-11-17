// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/WeapnSystem.h"

#include <string>

#include "DataAssets/AttackData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/CharacterSystem.h"


// Sets default values
AWeapnSystem::AWeapnSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FmodAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FmodAudioComp"));
	FmodAudioComp->SetupAttachment(RootComponent);
	FmodAudioComp->bAutoActivate = false;
	FmodAudioComp->bEnableTimelineCallbacks = true;
}

void AWeapnSystem::OnTimelineMarker(FString Name, int32 Position)
{
	// for ( Element : WeaponData->AttackMarkers)
	// {
	// 	
	// }
	FString Marker = AttackData->AttackMarkers[0].ToString() + " " + FString::FromInt(Level);
	if (Name.Contains(Marker, ESearchCase::IgnoreCase))
	{
		Attack();
	}
}

// Called when the game starts or when spawned
void AWeapnSystem::BeginPlay()
{
	Super::BeginPlay();
	Character = ACharacterSystem::GetCharacterInstance() ? ACharacterSystem::GetCharacterInstance() : nullptr;

	FmodAudioComp->SetEvent(AttackData->SoundEvent);
	FmodAudioComp->OnTimelineMarker.AddDynamic(this, &AWeapnSystem::OnTimelineMarker);
	FmodAudioComp->Play();
}

// Called every frame
void AWeapnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AWeapnSystem::GetEventPercentage()
{
	return static_cast<float>(FmodAudioComp->GetTimelinePosition()) / FmodAudioComp->GetLength(); 
}

void AWeapnSystem::SetPaused(bool Paused) const
{
	FmodAudioComp->SetPaused(Paused);
}

void AWeapnSystem::SetEventPercentage(float Percentage)
{
	FmodAudioComp->SetTimelinePosition(FmodAudioComp->GetLength() * Percentage);
}

void AWeapnSystem::Upgrade_Implementation()
{
}

void AWeapnSystem::Attack_Implementation()
{
}

