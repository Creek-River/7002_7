// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

//Behavior Tree関連のヘッダ追加
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

//Pawn Sensingに必要
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController(const class FObjectInitializer& PCIP);

protected:
	UPROPERTY(BlueprintReadWrite)
		class UBehaviorTree* bt; //Behavior Tree

	UPROPERTY(BlueprintReadWrite)
		class UBlackboardComponent* bb; //Blackboard

	virtual void BeginPlay() override; //Begin Playを宣言

	//Pawn Sensing 
	UPROPERTY(BlueprintReadWrite)
		class UPawnSensingComponent* PawnSensor;

	//Pawn Sensingがノイズを聴いたときに呼ぶコールバック
	UFUNCTION()
		void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);

	//Pawn SensingがPawnを視認した時に呼ぶコールバック
	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

	//タイマー用
	FTimerHandle timerHandle;
	void OnChaseTimeOut();



};
