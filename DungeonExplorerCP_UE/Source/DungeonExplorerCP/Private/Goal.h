// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h" // 必須
#include "Goal.generated.h"

UCLASS()
class AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	// 球体StaticMesh参照用コンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class UStaticMeshComponent* goalMeshComp;

	// オーバーラップイベントで実行するデリゲート
	UFUNCTION() // BPからは呼ばないが、これがないとデリゲートが呼ばれない
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, // ?
			const FHitResult &SweepResult);

											// オーバーラップ相手のアクタ
											// オーバーラップ相手のコンポーネント
											// オーバーラップ相手のIndex
	 // ヒット関連情報の詳細
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
