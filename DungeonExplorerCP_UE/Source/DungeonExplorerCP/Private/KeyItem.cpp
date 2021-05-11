// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyItem.h"


// Sets default values
AKeyItem::AKeyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();

	// keyItemMeshCompのオーバーラップイベントにデリゲート関数をバインドする
	keyItemMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AKeyItem::BeginOverlap);
	
}

// Called every frame
void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	yawAngle += rotateSpeed * DeltaTime; // 回転値のインクリメント。DeltaTimeの乗算は必須！
	keyItemMeshComp->SetRelativeRotation(FRotator(0.0f, yawAngle, 0.0f)); // Yawのみ更新
}

// オーバーラップイベントで実行するデリゲート
void AKeyItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	// nextBarrierActorの参照が取得できていたら、そのアクタを破壊する
	if (nextBarrierActor != nullptr)
		nextBarrierActor->Destroy();
	// キーアイテム自身も破壊する（実行順に注意）
	this->Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Got Key Item!!!"));
}

