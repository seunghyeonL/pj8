// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Types.h"
#include "pj8GameState.generated.h"

/**
 * 
 */

UCLASS()
class PJ8_API Apj8GameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	Apj8GameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	EWaveState WaveState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	EWaveType WaveType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<AActor*> SpawnVolumes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float WaveDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float WaitingDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevelNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	
	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle IntervalTimerHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver(bool bIsDead);
	
	void StartLevel();
	void OnCoinCollected();
	void OnWaveTimeUp();
	void EndLevel();
	
	void StartWave();
	void EndWave();
	
	void UpdateHUD();
};
