// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

/**
 * 
 */
UCLASS()
class PJ8_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	ASpartaGameState();

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	// int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevelNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	
	// UFUNCTION(BlueprintPure, Category = "Score")
	// int32 GetScore() const;
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
};





