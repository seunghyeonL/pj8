// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MineItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMineItem::AMineItem():
	ExplosionDelay{5.0f},
	ExplosionRadius{30.0f},
	ExplosionDamage{60}
{
	ItemType = "Mine";

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	// 5초 후 폭발 실행
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || !Actor->ActorHasTag("Player")) continue;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		                                 FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));
		// run Actor->TakeDamage() 
		UGameplayStatics::ApplyDamage(
			Actor,
			ExplosionDamage,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}

	DestroyItem();
}
