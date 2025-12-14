// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "UiManager.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CharacterSystem.h"

void UGameManager::StartGame() const
{
	GetSubsystem<UUiManager>()->ToggleCharacterSelection();
	UGameplayStatics::OpenLevel(this, "Level");
}
