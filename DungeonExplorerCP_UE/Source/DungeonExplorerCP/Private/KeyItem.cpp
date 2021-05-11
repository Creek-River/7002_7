// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyItem.h"


// Sets default values
AKeyItem::AKeyItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();

	// keyItemMeshComp�̃I�[�o�[���b�v�C�x���g�Ƀf���Q�[�g�֐����o�C���h����
	keyItemMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AKeyItem::BeginOverlap);
	
}

// Called every frame
void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	yawAngle += rotateSpeed * DeltaTime; // ��]�l�̃C���N�������g�BDeltaTime�̏�Z�͕K�{�I
	keyItemMeshComp->SetRelativeRotation(FRotator(0.0f, yawAngle, 0.0f)); // Yaw�̂ݍX�V
}

// �I�[�o�[���b�v�C�x���g�Ŏ��s����f���Q�[�g
void AKeyItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	// nextBarrierActor�̎Q�Ƃ��擾�ł��Ă�����A���̃A�N�^��j�󂷂�
	if (nextBarrierActor != nullptr)
		nextBarrierActor->Destroy();
	// �L�[�A�C�e�����g���j�󂷂�i���s���ɒ��Ӂj
	this->Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Got Key Item!!!"));
}

