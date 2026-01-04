// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/WeapnSystem.h"
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

	SoloFmodAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("SoloFmodAudioComp"));
	SoloFmodAudioComp->SetupAttachment(RootComponent);
	SoloFmodAudioComp->bAutoActivate = false;
	SoloFmodAudioComp->bEnableTimelineCallbacks = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AWeapnSystem::OnTimelineMarker(FString Name, int32 Position)
{
	if (!bShouldAttack)
		return;
	// UKismetSystemLibrary::PrintString(
	// 	GetWorld(),
	// 	Name,
	// 	true,
	// 	true,   // Print to log
	// 	FLinearColor::Green,
	// 	2.0f,    // Duration,
	// 	FName("aaaasaf")
	// );
	if (Name.Contains(AttackData->AttackMarkers[0].ToString() + " " + FString::FromInt(Level), ESearchCase::IgnoreCase))
		FirstMarkerAttack();
	else if (Name.Contains(AttackData->AttackMarkers[1].ToString() + " " + FString::FromInt(Level), ESearchCase::IgnoreCase))
		SecondMarkerAttack();
	else if (Name.Contains(AttackData->AttackMarkers[2].ToString() + " " + FString::FromInt(Level), ESearchCase::IgnoreCase))
		ThirdMarkerAttack();
	else if (Name.Contains(AttackData->AttackMarkers[3].ToString(), ESearchCase::IgnoreCase))
		FourthMarkerAttack();
}

void AWeapnSystem::SoloOnTimelineMarker(FString Name, int32 Position)
{
	// UKismetSystemLibrary::PrintString(
	// 	GetWorld(),
	// 	Name,
	// 	true,
	// 	true,   // Print to log
	// 	FLinearColor::Green,
	// 	2.0f,    // Duration,
	// 	FName("aaaafsaf")
	// );
	if (Name.Contains("Rise", ESearchCase::IgnoreCase))
		PreStartSoloPhase();
	else if (Name.Contains("Start", ESearchCase::IgnoreCase))
		StartSoloPhase();
	else if (Name.Contains("End", ESearchCase::IgnoreCase))
		EndSoloPhase();
	else if (Name.Contains(AttackData->SoloMarkers[0].ToString(), ESearchCase::IgnoreCase))
		FirstMarkerAttack();
	else if (Name.Contains(AttackData->SoloMarkers[1].ToString(), ESearchCase::IgnoreCase))
		SecondMarkerAttack();
	else if (Name.Contains(AttackData->SoloMarkers[2].ToString(), ESearchCase::IgnoreCase))
		ThirdMarkerAttack();
	else if (Name.Contains(AttackData->SoloMarkers[3].ToString(), ESearchCase::IgnoreCase))
		FourthMarkerAttack();
	
}

// Called when the game starts or when spawned
void AWeapnSystem::BeginPlay()
{
	Super::BeginPlay();
	Character = ACharacterSystem::GetCharacterInstance() ? ACharacterSystem::GetCharacterInstance() : nullptr;

	FmodAudioComp->SetEvent(AttackData->SoundEvent);
	FmodAudioComp->OnTimelineMarker.AddDynamic(this, &AWeapnSystem::OnTimelineMarker);
	FmodAudioComp->Play();

	SoloFmodAudioComp->SetEvent(AttackData->SoloEvent);
	SoloFmodAudioComp->OnTimelineMarker.AddDynamic(this, &AWeapnSystem::SoloOnTimelineMarker);
	
	RandomRotation = FQuat::MakeFromEuler(FVector(
		FMath::RandBool() ? FMath::RandRange(-RotationSafeDegree, RotationSafeDegree) : FMath::RandRange(180 - RotationSafeDegree, 180 + RotationSafeDegree),
		FMath::RandBool() ? FMath::RandRange(-RotationSafeDegree, RotationSafeDegree) : FMath::RandRange(180 - RotationSafeDegree, 180 + RotationSafeDegree),
		FMath::RandRange(0.0f, 360.0f)));
	RotationTime = 0;
	bShouldAttack = true;
	CurrentVolume = 1.f;
	FmodAudioComp->SetParameter(FName(TEXT("Volume")), CurrentVolume);
	bIsInSoloMode = false;
}

void AWeapnSystem::PreStartSoloPhase_Implementation()
{
}

void AWeapnSystem::StartSoloPhase_Implementation()
{
}

void AWeapnSystem::EndSoloPhase_Implementation()
{
}

void AWeapnSystem::SoloEnded_Implementation()
{
}

// Called every frame
void AWeapnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRotationAroundCharacter(DeltaTime);
	// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s, %f"), *GetName(), GetEventPercentage()), true, true, FColor::Red, 2,  FName(*(GetName() + TEXT("Weapon"))));
}

void AWeapnSystem::SecondMarkerAttack_Implementation()
{
}

void AWeapnSystem::FirstMarkerAttack_Implementation()
{
}

void AWeapnSystem::ThirdMarkerAttack_Implementation()
{
}

void AWeapnSystem::FourthMarkerAttack_Implementation()
{
}

void AWeapnSystem::SoloAttack_Implementation()
{
	SoloFmodAudioComp->SetTimelinePosition(SoloFmodAudioComp->GetLength() * 0);
	SoloFmodAudioComp->Play();
	SoloFmodAudioComp->OnEventStopped.AddDynamic(Character, &ACharacterSystem::StopSolo);
	bIsInSoloMode = true;
}

float AWeapnSystem::GetEventPercentage()
{
	return static_cast<float>(FmodAudioComp->GetTimelinePosition()) / FmodAudioComp->GetLength(); 
}

void AWeapnSystem::SetPaused(const bool Paused)
{
	bShouldAttack = !Paused;
	FmodAudioComp->SetPaused(Paused);
}

void AWeapnSystem::ToggleShouldAttack()
{
	bShouldAttack = !bShouldAttack;
}

void AWeapnSystem::SetEventPercentage(float Percentage)
{
	FmodAudioComp->SetTimelinePosition(FmodAudioComp->GetLength() * Percentage);
}

void AWeapnSystem::ChangeVolume(float Volume)
{
	CurrentVolume = FMath::Clamp(CurrentVolume + Volume, 0.0f, 1.0f);
	FmodAudioComp->SetParameter(FName(TEXT("Volume")), CurrentVolume);
}

void AWeapnSystem::UpdateRotationAroundCharacter(float DeltaTime)
{
	RotationTime += DeltaTime;
	float Angle = RotationTime * RotateSpeed;
	FVector Dir = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f);
	FVector ResultLocation = RandomRotation.RotateVector(Dir * RotateRadius) + Character->GetActorLocation() + FVector(0.0f, 0.0f, 160.0f);
	FVector MoveDirection = (RandomRotation.RotateVector(Dir * RotateRadius) + FVector(0.0f, 0.0f, 160.0f) - PrevLocationInOrbit).GetSafeNormal();
	PrevLocationInOrbit = RandomRotation.RotateVector(Dir * RotateRadius) + FVector(0.0f, 0.0f, 160.0f);
	SetActorRotation(MoveDirection.Rotation());
	SetActorLocation(ResultLocation);
}

void AWeapnSystem::Upgrade_Implementation()
{
}

void AWeapnSystem::Attack_Implementation()
{
}

