// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class PJ8_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMineItem();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;
	
	// 폭발까지 걸리는 시간 (5초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionDelay;
	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	float ExplosionRadius;
	// 폭발 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mine")
	int32 ExplosionDamage;

	// 지뢰 발동 여부
	FTimerHandle ExplosionTimerHandle;
	
	virtual void ActivateItem(AActor* Activator) override;

	void Explode();
};
