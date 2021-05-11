// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCPCharacter.h" //エネミーキャラクタを扱う上で必要

//コンストラクタ
AEnemyAIController::AEnemyAIController(const class FObjectInitializer& PCIP)
	:Super(PCIP)
{
	//BlackBoardコンポーネントを作成
	bb = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Black Board"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//waypoint取得の為、EnemyCharacterを取得
	AEnemyCPCharacter* enemyCharacter = Cast<AEnemyCPCharacter>(GetPawn());

	if (bt != NULL)
		RunBehaviorTree(bt); //BTを起動
	if (bb != NULL) {
		//waypointを取得　&& BBキー値として登録
		for (int i = 0; i < enemyCharacter->waypointTargets.Num(); i++) {
			FString key = "waypoint" + FString::FromInt(i + 1);
			bb->SetValueAsObject(*key, enemyCharacter->waypointTargets[i]);
		}
	}

	//Pawn Sensorのパラメータ設定
	PawnSensor->SightRadius = 900.0f; //視野半径
	PawnSensor->SensingInterval = 0.5f; //知覚の更新頻度
	PawnSensor->bEnableSensingUpdates = true;
	PawnSensor->bOnlySensePlayers = false; 
	PawnSensor->SetPeripheralVisionAngle(60.0f); //視野の広さ

	//Delegateを登録
	FScriptDelegate OnSeePawnDelegate;
	OnSeePawnDelegate.BindUFunction(this, "OnSeePawn");
	PawnSensor->OnSeePawn.Add(OnSeePawnDelegate);
	FScriptDelegate OnHearNoiseDelegate;
	OnHearNoiseDelegate.BindUFunction(this, "OnHearNoise");
	PawnSensor->OnHearNoise.Add(OnHearNoiseDelegate);
}

void AEnemyAIController::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	
}

void AEnemyAIController::OnSeePawn(APawn* OtherPawn)
{
	FString message = TEXT("Saw Pawn") + OtherPawn->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, message);

	bb->SetValueAsObject("Player", OtherPawn); //Playerキー値を入力
	//コールバックのタイマー発動
	GetWorld()->GetTimerManager().SetTimer(timerHandle, 
		this, &AEnemyAIController::OnChaseTimeOut, 3.0f, true);

}


void AEnemyAIController::OnChaseTimeOut()
{
	//キーの値をNULLに設定して巡回行動に戻す
	bb->SetValueAsObject("Player", NULL);
	//デバッグ出力
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Timeout");
}
