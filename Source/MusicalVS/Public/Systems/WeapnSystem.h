// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "WeapnSystem.generated.h"

class UAttackData;
class ACharacterSystem;

UCLASS()
class MUSICALVS_API AWeapnSystem : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	ACharacterSystem* Character;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UFMODAudioComponent* FmodAudioComp;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 0;

public:
	// Sets default values for this actor's properties
	AWeapnSystem();

private:
	UFUNCTION()
	void OnTimelineMarker(FString Name, int32 Position);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UAttackData> WeaponData;
	
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void Attack();
};
