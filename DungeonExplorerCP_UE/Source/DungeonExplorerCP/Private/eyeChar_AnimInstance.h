// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "eyeChar_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UeyeChar_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	// 空中にいるかどうかのフラグ。外から変更しないので、Read Onlyにしておく
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MomomoWorks")
		bool isInAir = false;

	// プレイヤーの移動スピード。外から変更しないので、Read Onlyにしておく
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MomomoWorks")
		float speed;

	// AnimaInstanceにおけるTick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
