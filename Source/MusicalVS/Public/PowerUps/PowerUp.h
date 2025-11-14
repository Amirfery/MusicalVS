// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolItem.h"
#include "PowerUp.generated.h"

class ACharacterSystem;

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	GemCollector		UMETA(DisplayName="Gem Collector"),
	Freeze				UMETA(DisplayName="Freeze")
};

UCLASS()
class MUSICALVS_API APowerUp : public APoolItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpType PowerUpType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CollidedWithPlayer(ACharacterSystem* PlayerSystem);

	UFUNCTION(BlueprintCallable)
	void CollectGems(ACharacterSystem* PlayerSystem);

	UFUNCTION(BlueprintCallable)
	void FreezeEnemies(ACharacterSystem* PlayerSystem);

	virtual void SetFloatValues_Implementation(const TArray<float>& FloatValues) override;
};
