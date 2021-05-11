// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DungeonExplorerCPCharacter.generated.h"

UCLASS(config=Game)
class ADungeonExplorerCPCharacter : public ACharacter
{
	GENERATED_BODY()

	/* "BlueprintReadWrite"に変更 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/* "BlueprintReadWrite"に変更 */
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

	// 追加プロパティ
	// キャラクタメッシュ(ActorComponent)の参照
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		UActorComponent* charMesh;
	// 爆発音アクタの参照
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		USoundBase* bangSound;
	// 爆発用パーティクルの参照
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		UParticleSystem* explosionParticle;

	FTimerHandle timerHandle; // タイマーハンドラ
	// リスポーン用関数。クラス内から呼ぶだけなので、UCLASS指定は不要
	void Respawn();

	// SuperSpeed関連
	// SuperSpeed管理ゲージ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float superSpeedPower = 1.0f;
	// 基準とする歩行速度。UE4エディタ側で調整できるようにしておく
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MomomoWorks")
		float walkSpeed = 300.0f;
	void OnSuperSpeedON(); // シフトキーダウンで呼ぶメソッド
	void OnSuperSpeedOFF(); // シフトキーアップで呼ぶメソッド
	bool isSuperSpeed = false; // 管理用フラグ

protected:

	// 親クラスのTickイベントを使うため、宣言しておく
	virtual void Tick(float DeltaTime) override;

	// Take Damageイベント（親クラスのオーバーライド）
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

