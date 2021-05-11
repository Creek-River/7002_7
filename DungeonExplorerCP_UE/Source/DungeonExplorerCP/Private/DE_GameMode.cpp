// Fill out your copyright notice in the Description page of Project Settings.

#include "DE_GameMode.h"
#include "UObject/ConstructorHelpers.h" // �R���e���c�u���E�U����A�Z�b�g���Q�Ƃ���ۂɕK�v

ADE_GameMode::ADE_GameMode() { // �R���X�g���N�^

	// �R���e���c�u���E�U����쐬����"eyeCharCharacter��T���A�ϐ��Ɋi�[
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/eyeCharCharacter"));
	if (PlayerPawnBPClass.Class != NULL) // BP�A�N�^�����������ꍇ
	{
		DefaultPawnClass = PlayerPawnBPClass.Class; // �f�t�H���g�Ń|�[��(�o��)������L�����Ɏw�肷��
	}

}

