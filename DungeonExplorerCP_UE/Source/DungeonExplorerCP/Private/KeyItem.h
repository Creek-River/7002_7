// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Static Mesh Component�����������ŕK�{
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h" 
#include "KeyItem.generated.h"

UCLASS()
class AKeyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyItem();

	// �L�[�A�C�e��StaticMesh�Q�Ɨp�̃R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class UStaticMeshComponent* keyItemMeshComp;

	// �A�C�e���擾��ɔj�󂷂�o���A�A�N�^�̎Q��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class AActor* nextBarrierActor;

	// �I�[�o�[���b�v�C�x���g�Ŏ��s����f���Q�[�g
	UFUNCTION()	// BP����͌Ă΂Ȃ����A���ꂪ�Ȃ��ƃf���Q�[�g���Ă΂�Ȃ�
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

	// ��]�X�s�[�h�����p�BUE4�G�f�B�^��BP�Œ����ł���悤��UPROPERTY�ɂ��Ă���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float rotateSpeed = 100.0f;
	// ��]����̂��ߎg�p�B�N���X�����Ŏg���̂ŁAUPROPERTY�łȂ��Ă�OK
	float yawAngle = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
};

