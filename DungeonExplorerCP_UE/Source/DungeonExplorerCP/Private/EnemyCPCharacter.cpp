// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCPCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" // �v���C���[�L�����N�^���擾�̂��߂ɕK�v

// Sets default values
AEnemyCPCharacter::AEnemyCPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// �N���X����Capsule Collier���擾���Ă���
	capsuleCollider = this->GetCapsuleComponent();
}

// Called when the game starts or when spawned
void AEnemyCPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FScriptDelegate Delegate; // �f���Q�[�g�ϐ����`
	Delegate.BindUFunction(this, "OnActorHit"); // OnActorHit�֐���o�^
	capsuleCollider->OnComponentHit.Add(Delegate); // Capsule Collider�̃f���Q�[�g�֐��Ƃ��ăo�C���h����
}

// �q�b�g�C�x���g�������ɃR�[�������C�x���g
void AEnemyCPCharacter::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FString otherName = OtherActor->GetFName().ToString(); // OtherActor����q�b�g����������E���A�C���X�^���X���̂��擾
	FString playerName = UGameplayStatics::GetPlayerCharacter(this, 0)->GetName(); // �v���C���[�̃C���X�^���X�����擾

	if (otherName == playerName) { // �C���X�^���X�� = �v���C���[�������ꍇ
		// �Ƃ肠�����R���\�[���o�͂��Ă���
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, UGameplayStatics::GetPlayerCharacter(this, 0)->GetName());

		// TakeDamage���s�p�̃p�����[�^�쐬�B����̓f�t�H���g�l�̂܂܂�OK
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		// ���݂̃Q�[���ɂ�����v���C���[�R���g���[�����擾
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		// �v���C���[�L�����N�^���擾���A���̃��\�b�h"Take Damage"�����s
		// GetPlayerCharacter��2�Ԃ߂̈����̓v���C���[��ID�B�}���`�v���C�̏ꍇ�͂���ID���g��������
		UGameplayStatics::GetPlayerCharacter(this, 0)->TakeDamage(1.0f, FDamageEvent(), PlayerController, this);
	}
}

// Called every frame
void AEnemyCPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


