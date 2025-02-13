// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"

#include <ThirdParty/SPIRV-Reflect/SPIRV-Reflect/spirv_reflect.h>

#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController() :
	InputMappingContext{nullptr},
	MoveAction{nullptr},
	JumpAction{nullptr},
	LookAction{nullptr},
	SprintAction{nullptr},
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	GameOverMenuWidgetClass(nullptr),
	GameOverMenuWidgetInstance(nullptr)
{
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (!HUDWidgetClass) return;

	HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	if (!HUDWidgetInstance) return;

	HUDWidgetInstance->AddToViewport();

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->UpdateHUD();
	}
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (!MainMenuWidgetClass) return;

	MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	if (!MainMenuWidgetInstance) return;

	MainMenuWidgetInstance->AddToViewport();

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("StartButtonText"));
	if (!ButtonText) return;

	if (bIsRestart)
	{
		ButtonText->SetText(FText::FromString(TEXT("Restart")));
	}
	else
	{
		ButtonText->SetText(FText::FromString(TEXT("Start")));
	}
}

void ASpartaPlayerController::ShowGameOverMenu(bool bIsDead)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverMenuWidgetInstance)
	{
		GameOverMenuWidgetInstance->RemoveFromParent();
		GameOverMenuWidgetInstance = nullptr;
	}

	if (!GameOverMenuWidgetClass) return;

	GameOverMenuWidgetInstance = CreateWidget<UUserWidget>(this, GameOverMenuWidgetClass);
	if (!GameOverMenuWidgetInstance) return;

	GameOverMenuWidgetInstance->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	UTextBlock* CongratulationText = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("CongratulationText"));
	if (!CongratulationText) return;

	if (bIsDead)
	{
		CongratulationText->SetText(FText::FromString(TEXT("Game Over!!")));
	}
	else
	{
		CongratulationText->SetText(FText::FromString(TEXT("Congratulation!!")));
	}
	
	UTextBlock* TotalScore = Cast<UTextBlock>(GameOverMenuWidgetInstance->GetWidgetFromName("TotalScore"));
	if (!TotalScore) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
	if (!SpartaGameInstance) return;

	TotalScore->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 1;
		SpartaGameInstance->TotalScore = 0;
	}

	// UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	if (!InputMappingContext)
	{
		return;
	}
	Subsystem->AddMappingContext(InputMappingContext, 0);


	// if (!HUDWidgetClass)
	// {
	// 	return;
	// }
	//
	// HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	// if (!HUDWidgetInstance)
	// {
	// 	return;
	// }
	// HUDWidgetInstance->AddToViewport();
	//
	// ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	// if (!SpartaGameState)
	// {
	// 	return;
	// }
	// SpartaGameState->UpdateHUD();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}
