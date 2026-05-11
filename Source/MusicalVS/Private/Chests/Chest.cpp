// Fill out your copyright notice in the Description page of Project Settings.


#include "Chests/Chest.h"


// Sets default values
AChest::AChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(100.0);
	CollectionSphere->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

void AChest::OpenChest()
{
	UE_LOG(LogTemp, Display, TEXT("Chest was opened!"));
	Destroy();
}


