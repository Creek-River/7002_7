// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockEmitter.generated.h"

UCLASS()
class ARockEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockEmitter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<class AStaticMeshActor> rockActor = nullptr;
	void SpawnRock(); // 内部で完結する関数のため、UCLASSマクロは使わない
	FTimerHandle timerHandle; // 時間差で岩をスポーンさせるためのタイマーハンドラ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Emitter Properties")
	float delayTime = 3.0f; // 岩のスポーン時間間隔

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
