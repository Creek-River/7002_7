// Fill out your copyright notice in the Description page of Project Settings.

#include "eyeChar_AnimInstance.h"
// CharacterMovement���������߂ɕK�{
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

void UeyeChar_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// �v���C���[��Pawn���擾
	APawn* eyeCharPawn = TryGetPawnOwner();

	if (eyeCharPawn) { // �v���C���[���������擾�ł��Ă���ꍇ
		// �ړ������Q�Ƃ��邽�߂�PawnMovementComponent���擾
		UPawnMovementComponent* movementComp = eyeCharPawn->GetMovementComponent();
		FVector velocity = movementComp->Velocity; // �ړ����x�i�x�N�g���j���擾
		speed = velocity.Size(); // �x�N�g�����i�ړ��X�s�[�h�j�𓾂�
		isInAir = movementComp->IsFalling(); // InFalling()�ŋ󒆂ɂ��邩�ǂ����擾�ł���
	}
}

