// Fill out your copyright notice in the Description page of Project Settings.


#include "Blessings/ThorBlessing.h"

#include "Engine/OverlapResult.h"
#include "Interfaces/Damageable.h"


// Sets default values
AThorBlessing::AThorBlessing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AThorBlessing::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThorBlessing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThorBlessing::ApplyEffect_Implementation()
{
	Super::ApplyEffect_Implementation();
	for (int i = 0; i < 1; i++)
	{
		FVector RandomOffset = FMath::VRand();
		RandomOffset.Z = 0.f;
		RandomOffset.Normalize();

		RandomOffset *= FMath::RandRange(1000.f, 3000.f);
	
		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
		// DrawDebugLine(
		// 	GetWorld(),
		// 	GetActorLocation() + RandomOffset + FVector(0.f, 0.f, 2000.f),
		// 	GetActorLocation() + RandomOffset,
		// 	FColor::Cyan,
		// 	false,
		// 	1.f,
		// 	0,
		// 	5.f
		// );
		DrawDebugDirectionalArrow(
			GetWorld(),
			GetActorLocation() + RandomOffset + FVector(0.f, 0.f, 2000.f),
			GetActorLocation() + RandomOffset,
			150.f,      
			FColor::Cyan,
			false,
			1.f,
			0,
			5.f
		);
		DrawDebugSphere(GetWorld(), GetActorLocation() + RandomOffset, 500.f, 32, FColor::Red, false, 1);

		GetWorld()->OverlapMultiByChannel(
			Overlaps,
			GetActorLocation() + RandomOffset,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			Sphere
		);

		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* Enemy = Result.GetActor();
			if (!IsValid(Enemy))
				continue;

			if (Enemy->Implements<UDamageable>())
			{
				IDamageable::Execute_TakeDamage(Enemy, Damage);
			}
		}
	}
}

