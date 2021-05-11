// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h" // �K�{
#include "Goal.generated.h"

UCLASS()
class AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	// ����StaticMesh�Q�Ɨp�R���|�[�l���g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		class UStaticMeshComponent* goalMeshComp;

	// �I�[�o�[���b�v�C�x���g�Ŏ��s����f���Q�[�g
	UFUNCTION() // BP����͌Ă΂Ȃ����A���ꂪ�Ȃ��ƃf���Q�[�g���Ă΂�Ȃ�
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, // ?
			const FHitResult &SweepResult);

											// �I�[�o�[���b�v����̃A�N�^
											// �I�[�o�[���b�v����̃R���|�[�l���g
											// �I�[�o�[���b�v�����Index
	 // �q�b�g�֘A���̏ڍ�
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
