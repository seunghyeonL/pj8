// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "KeyItem.generated.h"

UCLASS()
class PJ8_API AKeyItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKeyItem();
	
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp,
							   AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex,
							   bool bFromSweep,
							   const FHitResult& SweepResult) override;

	virtual void ActivateItem(AActor* Activator) override;
};
