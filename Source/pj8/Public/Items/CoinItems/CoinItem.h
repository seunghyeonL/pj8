// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseItem.h"
#include "GameFramework/Actor.h"
#include "CoinItem.generated.h"

UCLASS()
class PJ8_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 PointValue;
	
	virtual void ActivateItem(AActor* Activator) override;
};
