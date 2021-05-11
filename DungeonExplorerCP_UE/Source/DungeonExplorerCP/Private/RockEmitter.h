// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockEmitter.generated.h"

UCLASS()
class ARockEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARockEmitter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSubclassOf<class AStaticMeshActor> rockActor = nullptr;
	void SpawnRock(); // �����Ŋ�������֐��̂��߁AUCLASS�}�N���͎g��Ȃ�
	FTimerHandle timerHandle; // ���ԍ��Ŋ���X�|�[�������邽�߂̃^�C�}�[�n���h��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Emitter Properties")
	float delayTime = 3.0f; // ��̃X�|�[�����ԊԊu

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
