// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Items/BaseItem.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

ABaseItem* ASpawnVolume::SpawnRandomItem()
{
	FItemSpawnRow* SelectedRow = GetRandomItem();
	if (!SelectedRow) return nullptr;

	UClass* ActualClass = SelectedRow->ItemClass.Get();
	if (!ActualClass) return nullptr;

	return SpawnItem(ActualClass);
}

EWaveType ASpawnVolume::GetWaveType() const
{
	return WaveType;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	// 1) 박스 컴포넌트의 스케일된 Extent, 즉 x/y/z 방향으로 반지름(절반 길이)을 구함
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	// 2) 박스 중심 위치
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	// 3) 각 축별로 -Extent ~ +Extent 범위 랜덤값 생성
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;

	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0, TotalChance);
	float AccumulatedChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulatedChance += Row->SpawnChance;
		if (RandValue < AccumulatedChance)
		{
			return Row;
		}
	}

	return nullptr;
}

ABaseItem* ASpawnVolume::SpawnItem(TSubclassOf<ABaseItem> ItemClass)
{
	if (!ItemClass) return nullptr;

	ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);

	SpawnedItems.Push(SpawnedItem);
	return SpawnedItem;
}

void ASpawnVolume::DestroySpawnedItems()
{
	for (AActor* SpawnedItemBeforeCast : SpawnedItems)
	{
		if (ABaseItem* SpawnedItem = Cast<ABaseItem>(SpawnedItemBeforeCast))
		{
			SpawnedItem->DestroyItem();
		}
	}
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}
