// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "GameFramework/Actor.h"
#include "SoulNote.generated.h"

class ACharacterSystem;

UCLASS()
class MUSICALVS_API ASoulNote : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoulNote();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void CollidedWithPlayer(ACharacterSystem* PlayerSystem);

	UFUNCTION(BlueprintCallable)
	void Collect(ACharacterSystem* Player);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SoulNoteAmount;
	
protected:
	bool bIsMovingToPlayer;
	float MoveSpeed;

	UPROPERTY()
	TObjectPtr<ACharacterSystem> TargetPlayer;
};
