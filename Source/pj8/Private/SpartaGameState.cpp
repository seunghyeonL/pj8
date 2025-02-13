// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Items/CoinItems/CoinItem.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	// Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevelNum = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	// UpdateHUD();
	
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}

void ASpartaGameState::StartLevel()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	SpartaPlayerController->ShowGameHUD();
	
	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;
	
	for (AActor* FoundVolume : FoundVolumes)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolume);
		if (!SpawnVolume) continue;

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
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

	UpdateHUD();

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
	       CurrentLevelIndex + 1,
	       SpawnedCoinCount
	);
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Collected %d coins / %d"),
	       CollectedCoinCount,
	       SpawnedCoinCount
	);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentLevelIndex++;

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	// AddScore(Score);
	SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
	
	if (CurrentLevelIndex >= MaxLevelNum)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

// int32 ASpartaGameState::GetScore() const
// {
// 	return Score;
// }

void ASpartaGameState::AddScore(int32 Amount)
{
	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetWorld()->GetGameInstance());
	if (!SpartaGameInstance) return;

	SpartaGameInstance->AddToScore(Amount);
}

void ASpartaGameState::OnGameOver()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	SpartaPlayerController->ShowMainMenu(true);
}

void ASpartaGameState::UpdateHUD()
{
	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PlayerController) return;

	ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController);
	if (!SpartaPlayerController) return;

	UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget();
	if (!HUDWidget) return;

	UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Time"));
	if (!TimeText) return;

	float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
	TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
	UE_LOG(LogTemp, Display, TEXT("Time: %.1f"), RemainingTime);

	UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("Score"));
	if (!ScoreText) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;

	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
	if (!SpartaGameInstance) return;

	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));

	UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level")));
	if (!LevelIndexText) return;

	LevelIndexText->SetText(
		FText::FromString(FString::Printf(TEXT("Level: %d"), SpartaGameInstance->CurrentLevelIndex + 1)));
}
