// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "WeapnSystem.generated.h"

struct FAttackLevelStruct;
class UAttackData;
class ACharacterSystem;

UCLASS()
class MUSICALVS_API AWeapnSystem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	ACharacterSystem* Character;

	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UFMODAudioComponent* FmodAudioComp;

public:
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttackData> AttackData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Id;

public:
	// Sets default values for this actor's properties
	AWeapnSystem();

private:
	UFUNCTION()
	void OnTimelineMarker(FString Name, int32 Position);

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Upgrade();

	UFUNCTION(BlueprintNativeEvent)
	void Attack();

	float GetEventPercentage();

	void SetPaused(bool Paused) const;
	void SetEventPercentage(float Percentage);
};
