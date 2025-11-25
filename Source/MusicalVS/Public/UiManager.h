// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UiManager.generated.h"


UCLASS(Blueprintable)
class MUSICALVS_API UUiManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UUserWidget> MainMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UUserWidget> GameOverWidget;
	
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void ToggleMainMenu();

	UFUNCTION(BlueprintCallable)
	void ToggleGameOver();

	UFUNCTION(BlueprintCallable)
	void ShowWidget(UUserWidget* Widget) const;
	
	UFUNCTION(BlueprintCallable)
	void HideWidget(UUserWidget* Widget) const;

};
