// Fill out your copyright notice in the Description page of Project Settings.

#include "DE_GameMode.h"
#include "UObject/ConstructorHelpers.h" // コンテンツブラウザからアセットを参照する際に必要

ADE_GameMode::ADE_GameMode() { // コンストラクタ

	// コンテンツブラウザから作成した"eyeCharCharacterを探し、変数に格納
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/eyeCharCharacter"));
	if (PlayerPawnBPClass.Class != NULL) // BPアクタが見つかった場合
	{
		DefaultPawnClass = PlayerPawnBPClass.Class; // デフォルトでポーン(出現)させるキャラに指定する
	}

}

