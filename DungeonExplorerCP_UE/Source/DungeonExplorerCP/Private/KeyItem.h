// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Static Mesh Componentを扱ううえで必須
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h" 
#include "KeyItem.generated.h"

UCLASS()
class AKeyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyItem();

	// キーアイテムStaticMesh参照用のコンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class UStaticMeshComponent* keyItemMeshComp;

	// アイテム取得後に破壊するバリアアクタの参照
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class AActor* nextBarrierActor;

	// オーバーラップイベントで実行するデリゲート
	UFUNCTION()	// BPからは呼ばないが、これがないとデリゲートが呼ばれない
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

	// 回転スピード調整用。UE4エディタやBPで調整できるようにUPROPERTYにしておく
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float rotateSpeed = 100.0f;
	// 回転動作のため使用。クラス内部で使うので、UPROPERTYでなくてもOK
	float yawAngle = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
};

