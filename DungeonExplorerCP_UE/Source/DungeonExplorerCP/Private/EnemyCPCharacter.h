// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// �L�����N�^�̃q�b�g�������s�������ŕK�{
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
//�^�[�Q�b�g�|�C���g��������ŕK�{
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "EnemyCPCharacter.generated.h"

UCLASS()
class AEnemyCPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCPCharacter();

	//Behavior Tree�Ŏg�p���鏄��ړI�n
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		TArray<ATargetPoint*> waypointTargets;

	// �L�����N�^��Capsule Collider�̎Q�Ɓi�|�C���^�j
	UPROPERTY(BlueprintReadWrite, Category = "MomomoWorks")
		UCapsuleComponent* capsuleCollider;

	// �q�b�g�C�x���g�������Ɏ��s����f���Q�[�g���\�b�h
	// Capsule Collider�̎d�l�ɍ��킹�������w�肪�K�v
	UFUNCTION()
		void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
