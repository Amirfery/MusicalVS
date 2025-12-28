#include "Systems/BlessingSystem.h"

#include "FMODAudioComponent.h"
#include "DataAssets/BlessingData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/CharacterSystem.h"


ABlessingSystem::ABlessingSystem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FmodAudioComp = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FmodAudioComp"));
	FmodAudioComp->SetupAttachment(RootComponent);
	FmodAudioComp->bAutoActivate = false;
	FmodAudioComp->bEnableTimelineCallbacks = true;

	// StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	// StaticMesh->SetupAttachment(RootComponent);
}


void ABlessingSystem::OnTimelineMarker(FString Name, int32 Position)
{
	FString Marker = BlessingData->BlessingMarkers[0].ToString() + " " + FString::FromInt(Level);
	if (Name.Contains(BlessingData->BlessingMarkers[0].ToString(), ESearchCase::IgnoreCase))
	{
		ApplyEffect();
	}
}

void ABlessingSystem::BeginPlay()
{
	Super::BeginPlay();
	Character = ACharacterSystem::GetCharacterInstance() ? ACharacterSystem::GetCharacterInstance() : nullptr;

	FmodAudioComp->SetEvent(BlessingData->SoundEvent);
	FmodAudioComp->OnTimelineMarker.AddDynamic(this, &ABlessingSystem::OnTimelineMarker);
	FmodAudioComp->Play();
	
}

void ABlessingSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s, %f"), *GetName(), static_cast<float>(FmodAudioComp->GetTimelinePosition()) / FmodAudioComp->GetLength()), true, true, FColor::Red, 2,  FName(*(GetName() + TEXT("Weapon"))));

}

void ABlessingSystem::SetPaused(bool Paused)
{
	FmodAudioComp->SetPaused(Paused);
}

void ABlessingSystem::SetEventPercentage(float Percentage)
{
	FmodAudioComp->SetTimelinePosition(FmodAudioComp->GetLength() * Percentage);
}

void ABlessingSystem::Upgrade_Implementation()
{
}

void ABlessingSystem::ApplyEffect_Implementation()
{
}

