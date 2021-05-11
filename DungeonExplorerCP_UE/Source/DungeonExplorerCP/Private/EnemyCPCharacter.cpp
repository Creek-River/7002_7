// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCPCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" // プレイヤーキャラクタ情報取得のために必要

// Sets default values
AEnemyCPCharacter::AEnemyCPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// クラス内のCapsule Collierを取得しておく
	capsuleCollider = this->GetCapsuleComponent();
}

// Called when the game starts or when spawned
void AEnemyCPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FScriptDelegate Delegate; // デリゲート変数を定義
	Delegate.BindUFunction(this, "OnActorHit"); // OnActorHit関数を登録
	capsuleCollider->OnComponentHit.Add(Delegate); // Capsule Colliderのデリゲート関数としてバインドする
}

// ヒットイベント発生時にコールされるイベント
void AEnemyCPCharacter::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FString otherName = OtherActor->GetFName().ToString(); // OtherActorからヒットした相手を拾い、インスタンス名称を取得
	FString playerName = UGameplayStatics::GetPlayerCharacter(this, 0)->GetName(); // プレイヤーのインスタンス名を取得

	if (otherName == playerName) { // インスタンス名 = プレイヤーだった場合
		// とりあえずコンソール出力しておく
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, UGameplayStatics::GetPlayerCharacter(this, 0)->GetName());

		// TakeDamage実行用のパラメータ作成。今回はデフォルト値のままでOK
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		// 現在のゲームにおけるプレイヤーコントローラを取得
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		// プレイヤーキャラクタを取得し、そのメソッド"Take Damage"を実行
		// GetPlayerCharacterの2番めの引数はプレイヤーのID。マルチプレイの場合はこのIDを使い分ける
		UGameplayStatics::GetPlayerCharacter(this, 0)->TakeDamage(1.0f, FDamageEvent(), PlayerController, this);
	}
}

// Called every frame
void AEnemyCPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


