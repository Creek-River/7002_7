// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DE_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONEXPLORERCP_API UDE_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// �v���C���[�c�@
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		int32 playerNum = 3;
		
};
