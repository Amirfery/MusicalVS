// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlessingSystem.generated.h"

class UBlessingData;
class UAttackData;
class ACharacterSystem;
class UFMODAudioComponent;

UCLASS()
class MUSICALVS_API ABlessingSystem : public AActor
{
	GENERATED_BODY()

public:
	ABlessingSystem();

protected:
	UPROPERTY(EditAnywhere)
	ACharacterSystem* Character;
	
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UFMODAudioComponent* FmodAudioComp;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UBlessingData> BlessingData;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Id;

private:
	UFUNCTION()
	virtual void OnTimelineMarker(FString Name, int32 Position);
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void ApplyEffect();

	UFUNCTION(BlueprintNativeEvent)
	void Upgrade();

	void SetPaused(bool Paused);
	void SetEventPercentage(float Percentage);
};
