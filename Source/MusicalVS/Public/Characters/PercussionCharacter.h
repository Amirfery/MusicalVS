// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/CharacterSystem.h"
#include "PercussionCharacter.generated.h"

UCLASS()
class MUSICALVS_API APercussionCharacter : public ACharacterSystem
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APercussionCharacter();
	
	virtual void FirstMarkerAttack_Implementation() override;
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> AuraMesh;
	
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
	
	UPROPERTY(Transient)
	bool bIsInAttack = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LightThreshold= 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StrongThreshold = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinThreshold = 0.1f;
	
	UPROPERTY(Transient)
	float CurrentThreshold;
	
	UPROPERTY(Transient)
	float Alpha;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ChangeSpeed = 10;
};
