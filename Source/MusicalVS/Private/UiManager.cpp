// Fill out your copyright notice in the Description page of Project Settings.


#include "UiManager.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CharacterSystem.h"


void UUiManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// MainMenuWidgetClass = LoadClass<UUserWidget>(nullptr,TEXT("/Game/UI/Widgets/MainMenu/WBP_MainMenuPanel.WBP_MainMenuPanel_C"));
	//
	// GameOverWidgetClass =  LoadClass<UUserWidget>(nullptr,TEXT("/Game/UI/Widgets/WBP_GameOver.WBP_GameOver_C"));
}

void UUiManager::ToggleMainMenu()
{
	if (!MainMenuWidget)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		ShowWidget(MainMenuWidget);
	}
	else
	{
		HideWidget(MainMenuWidget);
		MainMenuWidget = nullptr;
	}
}

void UUiManager::ToggleGameOver()
{
	if (!GameOverWidget)
	{
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		ShowWidget(GameOverWidget);
		ACharacterSystem::GetCharacterInstance()->SetPaused(true);
	}
	else
	{
		HideWidget(GameOverWidget);
		GameOverWidget = nullptr;
		ACharacterSystem::GetCharacterInstance()->SetPaused(false);
	}
}

void UUiManager::ToggleCharacterSelection()
{
	if (!CharacterSelectionWidget)
	{
		CharacterSelectionWidget = CreateWidget<UUserWidget>(GetWorld(), CharacterSelectionWidgetClass);
		ToggleMainMenu();
		ShowWidget(CharacterSelectionWidget);
	}
	else
	{
		HideWidget(CharacterSelectionWidget);
		CharacterSelectionWidget = nullptr;
	}
}

void UUiManager::ShowWidget(UUserWidget* Widget) const
{
	Widget->AddToViewport();
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	PC->bShowMouseCursor = true;
	PC->SetInputMode(FInputModeUIOnly());

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UUiManager::HideWidget(UUserWidget* Widget) const
{
	Widget->RemoveFromParent();
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	PC->bShowMouseCursor = false;
	PC->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
