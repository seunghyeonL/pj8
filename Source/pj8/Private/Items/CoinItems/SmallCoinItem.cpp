// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CoinItems/SmallCoinItem.h"


// Sets default values
ASmallCoinItem::ASmallCoinItem()
{
	PointValue = 10;
	ItemType = "SmallCoin";
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}

