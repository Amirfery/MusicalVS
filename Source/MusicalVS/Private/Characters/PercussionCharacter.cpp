// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PercussionCharacter.h"


// Sets default values
APercussionCharacter::APercussionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AuraMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("AuraMesh"));
	AuraMesh->SetupAttachment(RootComponent);
	AuraMesh->SetAbsolute(false, true);
	AuraMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APercussionCharacter::FirstMarkerAttack_Implementation()
{
	Super::FirstMarkerAttack_Implementation();
	CurrentThreshold = LightThreshold;
	bIsInAttack = true;
}

void APercussionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Alpha = MinThreshold;
	CurrentThreshold = LightThreshold;
	DynamicMat = AuraMesh->CreateAndSetMaterialInstanceDynamic(0);
	DynamicMat->SetScalarParameterValue(FName("Beat"), Alpha);
}

void APercussionCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	float Progress = DeltaSeconds * ChangeSpeed;
	float PreviousAlpha = Alpha;
	if (bIsInAttack)
	{
		Alpha = FMath::Clamp(Alpha + Progress, MinThreshold, CurrentThreshold);
		if (Alpha == CurrentThreshold)
		{
			bIsInAttack = false;
		}
	}
	else
	{
		Alpha = FMath::Clamp(Alpha - Progress, MinThreshold, CurrentThreshold);
	}
	
	if (Alpha != PreviousAlpha)
	{
		DynamicMat->SetScalarParameterValue(FName("Beat"), Alpha);
	}
}

