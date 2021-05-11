// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DungeonExplorerCPCharacter.generated.h"

UCLASS(config=Game)
class ADungeonExplorerCPCharacter : public ACharacter
{
	GENERATED_BODY()

	/* "BlueprintReadWrite"�ɕύX */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/* "BlueprintReadWrite"�ɕύX */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ADungeonExplorerCPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// �ǉ��v���p�e�B
	// �L�����N�^���b�V��(ActorComponent)�̎Q��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		UActorComponent* charMesh;
	// �������A�N�^�̎Q��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		USoundBase* bangSound;
	// �����p�p�[�e�B�N���̎Q��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		UParticleSystem* explosionParticle;

	FTimerHandle timerHandle; // �^�C�}�[�n���h��
	// ���X�|�[���p�֐��B�N���X������ĂԂ����Ȃ̂ŁAUCLASS�w��͕s�v
	void Respawn();

	// SuperSpeed�֘A
	// SuperSpeed�Ǘ��Q�[�W
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float superSpeedPower = 1.0f;
	// ��Ƃ�����s���x�BUE4�G�f�B�^���Œ����ł���悤�ɂ��Ă���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float walkSpeed = 300.0f;
	void OnSuperSpeedON(); // �V�t�g�L�[�_�E���ŌĂԃ��\�b�h
	void OnSuperSpeedOFF(); // �V�t�g�L�[�A�b�v�ŌĂԃ��\�b�h
	bool isSuperSpeed = false; // �Ǘ��p�t���O

protected:

	// �e�N���X��Tick�C�x���g���g�����߁A�錾���Ă���
	virtual void Tick(float DeltaTime) override;

	// Take Damage�C�x���g�i�e�N���X�̃I�[�o�[���C�h�j
	virtual float TakeDamage(float Damage,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		class AActor* DamageCauser) override;


	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

