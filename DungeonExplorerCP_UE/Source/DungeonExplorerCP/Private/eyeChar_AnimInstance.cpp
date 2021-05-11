// Fill out your copyright notice in the Description page of Project Settings.

#include "eyeChar_AnimInstance.h"
// CharacterMovementを扱うために必須
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

void UeyeChar_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// プレイヤーのPawnを取得
	APawn* eyeCharPawn = TryGetPawnOwner();

	if (eyeCharPawn) { // プレイヤーが正しく取得できている場合
		// 移動情報を参照するためのPawnMovementComponentを取得
		UPawnMovementComponent* movementComp = eyeCharPawn->GetMovementComponent();
		FVector velocity = movementComp->Velocity; // 移動速度（ベクトル）を取得
		speed = velocity.Size(); // ベクトル長（移動スピード）を得る
		isInAir = movementComp->IsFalling(); // InFalling()で空中にいるかどうか取得できる
	}
}

