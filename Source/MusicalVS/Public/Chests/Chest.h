// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

UCLASS()
class MUSICALVS_API AChest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChest();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollectionSphere;
	
	UFUNCTION(BlueprintCallable)
	void OpenChest();
};
