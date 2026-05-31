// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyAudio.generated.h"

class UEnemyData;
class UFMODAudioComponent;

UCLASS()
class MUSICALVS_API AEnemyAudio : public AActor
{
	GENERATED_BODY()
	
	UFUNCTION()
	void StartPlaying();

public:
	// Sets default values for this actor's properties
	AEnemyAudio();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void Initialize(UEnemyData* NewEnemyData);
	
	UFUNCTION(BlueprintCallable)
	void SetPaused(bool Paused);
	
	UFUNCTION(BlueprintCallable)
	void SetEventPercentage(float Percentage);
	
	UFUNCTION(BlueprintCallable)
	void ChangeVolume(float Volume);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudio;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentVolume;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UEnemyData> EnemyData;
	
	
};
