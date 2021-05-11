// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// キャラクタのヒット処理を行ううえで必須
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
//ターゲットポイントを扱う上で必須
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "EnemyCPCharacter.generated.h"

UCLASS()
class AEnemyCPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCPCharacter();

	//Behavior Treeで使用する巡回目的地
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		TArray<ATargetPoint*> waypointTargets;

	// キャラクタのCapsule Colliderの参照（ポインタ）
	UPROPERTY(BlueprintReadWrite, Category = "MomomoWorks")
		UCapsuleComponent* capsuleCollider;

	// ヒットイベント発生時に実行するデリゲートメソッド
	// Capsule Colliderの仕様に合わせた引数指定が必要
	UFUNCTION()
		void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
