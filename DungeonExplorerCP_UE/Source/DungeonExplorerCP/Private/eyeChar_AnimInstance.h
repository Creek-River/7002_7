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

	// �󒆂ɂ��邩�ǂ����̃t���O�B�O����ύX���Ȃ��̂ŁARead Only�ɂ��Ă���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MomomoWorks")
		bool isInAir = false;

	// �v���C���[�̈ړ��X�s�[�h�B�O����ύX���Ȃ��̂ŁARead Only�ɂ��Ă���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MomomoWorks")
		float speed;

	// AnimaInstance�ɂ�����Tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
