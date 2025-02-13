// // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 *
 */
UCLASS()
class PJ8_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASpartaPlayerController();

	// IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	// IA_Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	// IA_Look
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

	// UMG Widget Class From Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	// Menu UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MENU")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MENU")
	UUserWidget* MainMenuWidgetInstance;

	// GameOver UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOver")
	TSubclassOf<UUserWidget> GameOverMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameOver")
	UUserWidget* GameOverMenuWidgetInstance;
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	// Show HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	// Show MainMenu
	UFUNCTION(BlueprintCallable, Category = "MENU")
	void ShowMainMenu(bool bIsRestart);
	// Show GameOver
	UFUNCTION(BlueprintCallable, Category = "MENU")
	void ShowGameOverMenu(bool bIsDead);
	
	UFUNCTION(BlueprintCallable, Category = "MENU")
	void StartGame();

protected:
	virtual void BeginPlay() override;
};
