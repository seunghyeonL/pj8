// Fill out your copyright notice in the Description page of Project Settings.


#include "pj8GameState.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Items/CoinItems/CoinItem.h"
#include "Kismet/GameplayStatics.h"

Apj8GameState::Apj8GameState()
{
	// Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	WaveDuration = 3.0f;
	WaitingDuration = 5.0f;
	CurrentLevelIndex = 0;
	MaxLevelNum = 1;
}

void Apj8GameState::BeginPlay()
{
	Super::BeginPlay();

	// UpdateHUD();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&Apj8GameState::UpdateHUD,
		0.1f,
		true
	);
}

void Apj8GameState::AddScore(int32 Amount)
{
	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	SpartaGameInstance->AddToScore(Amount);
}

void Apj8GameState::OnGameOver(bool bIsDead)
{
	WaveState = EWaveState::Wave_End;
	WaveType = EWaveType::Wave_End;
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	// SpartaPlayerController->ShowMainMenu(true);
	SpartaPlayerController->ShowGameOverMenu(bIsDead);
}

void Apj8GameState::StartLevel()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	SpartaPlayerController->ShowGameHUD();

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;

	WaveType = EWaveType::Wave_1;
	WaveState = EWaveState::Wave_Waiting;

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), SpawnVolumes);

	if (CurrentLevelIndex > 0)
	{
		GetWorldTimerManager().SetTimer(
			IntervalTimerHandle,
			this,
			&Apj8GameState::StartWave,
			WaitingDuration,
			false
		);
	}
}

void Apj8GameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Collected %d coins / %d"),
	       CollectedCoinCount,
	       SpawnedCoinCount
	);
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		EndWave();
	}
}

void Apj8GameState::OnWaveTimeUp()
{
	EndWave();
}

void Apj8GameState::EndLevel()
{
	// GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentLevelIndex++;

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	// AddScore(Score);
	SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;

	if (CurrentLevelIndex > MaxLevelNum)
	{
		OnGameOver(false);
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver(false);
	}
}

void Apj8GameState::StartWave()
{
	WaveState = EWaveState::Wave_Running;

	const int32 ItemToSpawn{
		WaveType == EWaveType::Wave_1 ? 20 : WaveType == EWaveType::Wave_2 ? 30 : WaveType == EWaveType::Wave_3 ? 40 : 0
	};

	for (AActor* SpawnVolumeBeforeCast : SpawnVolumes)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(SpawnVolumeBeforeCast);
		if (!SpawnVolume || SpawnVolume->GetWaveType() != WaveType) continue;

		for (int32 i = 0; i < ItemToSpawn; i++)
		{
			AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
			if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
			{
				SpawnedCoinCount++;
			}
		}
	}
	
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&Apj8GameState::OnWaveTimeUp,
		WaveDuration,
		false
	);
}

void Apj8GameState::EndWave()
{
	for (AActor* SpawnVolumeBeforeCast : SpawnVolumes)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(SpawnVolumeBeforeCast);
		if (!SpawnVolume || SpawnVolume->GetWaveType() != WaveType) continue;

		SpawnVolume->DestroySpawnedItems();
	}

	if (WaveType == EWaveType::Wave_3)
	{
		WaveState = EWaveState::Wave_End;
		WaveType = EWaveType::Wave_End;
		EndLevel();
	}
	else
	{
		WaveState = EWaveState::Wave_Waiting;
		WaveType = EWaveType(int32(WaveType) + 1);
		
		GetWorldTimerManager().SetTimer(
			IntervalTimerHandle,
			this,
			&Apj8GameState::StartWave,
			WaitingDuration,
			false
		);
	}
}

void Apj8GameState::UpdateHUD()
{
	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();
	if (!HUDWidget) return;

	UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Time"));
	if (!TimeText) return;
	float WaveRemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
	TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), WaveRemainingTime)));
	UE_LOG(LogTemp, Display, TEXT("Time: %.1f"), WaveRemainingTime);

	UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Score"));
	if (!ScoreText) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
	if (!SpartaGameInstance) return;

	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));

	UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level")));
	if (!WaveIndexText) return;

	switch (WaveType)
	{
	case EWaveType::Wave_1:
		WaveIndexText->SetText(FText::FromString(TEXT("Wave 1")));
		break;
	case EWaveType::Wave_2:
		WaveIndexText->SetText(FText::FromString(TEXT("Wave 2")));
		break;
	case EWaveType::Wave_3:
		WaveIndexText->SetText(FText::FromString(TEXT("Wave 3")));
		break;
	case EWaveType::Wave_End:
		break;
	}

	UTextBlock* WaitingTimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaitingTime")));
	if (!WaitingTimeText) return;

	switch (WaveState)
	{
	case EWaveState::Wave_Waiting:
		{
			float WaitingRemainingTime = GetWorldTimerManager().GetTimerRemaining(IntervalTimerHandle);
			WaitingTimeText->SetText(FText::FromString(FString::Printf(TEXT("Waiting: %.1f"), WaitingRemainingTime)));
			if (WaitingTimeText->GetVisibility() != ESlateVisibility::Visible)
			{
				WaitingTimeText->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		}
	case EWaveState::Wave_Running:
		if (WaitingTimeText->GetVisibility() != ESlateVisibility::Hidden)
		{
			WaitingTimeText->SetVisibility(ESlateVisibility::Hidden);
		}
		break;
	case EWaveState::Wave_End:
		break;
	}
}
