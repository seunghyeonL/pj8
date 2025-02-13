// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/HealingItem.h"
#include "Characters/SpartaCharacter.h"

// Sets default values
AHealingItem::AHealingItem():
	HealAmount{20}
{
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (!Activator || !Activator->ActorHasTag("Player")) return;

	ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator);
	if (!PlayerCharacter) return;

	PlayerCharacter->AddHealth(HealAmount);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(
		                                 TEXT("Player gained %d HP!"),
		                                 HealAmount));

	DestroyItem();
}
