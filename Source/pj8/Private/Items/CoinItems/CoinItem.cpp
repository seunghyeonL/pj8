// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/CoinItems/CoinItem.h"

#include "pj8GameState.h"
#include "SpartaGameState.h"

// Sets default values
ACoinItem::ACoinItem():
	PointValue(0)
{
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (!Activator || !Activator->ActorHasTag("Player")) return;

	UWorld* World = Activator->GetWorld();
	if (!World) return;

	// ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>();
	Apj8GameState* GameState = World->GetGameState<Apj8GameState>();
	if (!GameState) return;

	GameState->AddScore(PointValue);
	GameState->OnCoinCollected();
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(
		                                 TEXT("Player gained %d points!"),
		                                 PointValue
	                                 ));
	
	DestroyItem();
}
