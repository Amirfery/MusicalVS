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
}

// Called when the game starts or when spawned
void AWeapnSystem::BeginPlay()
{
	Super::BeginPlay();
	Character = ACharacterSystem::GetCharacterInstance() ? ACharacterSystem::GetCharacterInstance() : nullptr;

	FmodAudioComp->SetEvent(AttackData->SoundEvent);
	FmodAudioComp->OnTimelineMarker.AddDynamic(this, &AWeapnSystem::OnTimelineMarker);
	FmodAudioComp->Play();
	RandomRotation = FQuat::MakeFromEuler(FVector(
		FMath::RandBool() ? FMath::RandRange(-RotationSafeDegree, RotationSafeDegree) : FMath::RandRange(180 - RotationSafeDegree, 180 + RotationSafeDegree),
		FMath::RandBool() ? FMath::RandRange(-RotationSafeDegree, RotationSafeDegree) : FMath::RandRange(180 - RotationSafeDegree, 180 + RotationSafeDegree),
		FMath::RandRange(0.0f, 360.0f)));
	RotationTime = 0;
	bShouldAttack = true;
}

// Called every frame
void AWeapnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRotationAroundCharacter(DeltaTime);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s, %f"), *GetName(), GetEventPercentage()), true, true, FColor::Red, 2,  FName(*(GetName() + TEXT("Weapon"))));
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

void AWeapnSystem::StartSolo_Implementation()
{
	FmodAudioComp->SetEvent(AttackData->SoloEvent);
	FmodAudioComp->Play();
}

void AWeapnSystem::FourthMarketAttack_Implementation()
{
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

void AWeapnSystem::SetEventPercentage(float Percentage)
{
	FmodAudioComp->SetTimelinePosition(FmodAudioComp->GetLength() * Percentage);
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

