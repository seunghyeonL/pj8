// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseItem.h"

#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Item isn't run with tick.
	PrimaryActorTick.bCanEverTick = false;

	// Root Component
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// Collision
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	// Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	// Overlap Event Binding
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}

// 플레이어가 아이템 범위에 들어왔을 때 동작
void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp,
                              AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex,
                              bool bFromSweep,
                              const FHitResult& SweepResult)
{
	// OtherActor Player Check
	if (!OtherActor || !OtherActor->ActorHasTag("player"))
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf((TEXT("OverLap!!!"))));
	// ITem Using 
	ActivateItem(OtherActor);
}

// 플레이어가 아이템 범위를 벗어났을 때 동작
void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex)
{
	// 기본은 빈 함수 - 필요하다면 자식 클래스에서 활용
}

// 아이템이 사용(Activate)되었을 때 동작
void ABaseItem::ActivateItem(AActor* Activator)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf((TEXT("Activate!"))));
}

// 아이템 유형을 반환
FName ABaseItem::GetItemType() const
{
	return ItemType;
}

// 아이템을 파괴(제거)하는 함수
void ABaseItem::DestroyItem()
{
	// AActor에서 제공하는 Destroy() 함수로 객체 제거
	Destroy();
}
