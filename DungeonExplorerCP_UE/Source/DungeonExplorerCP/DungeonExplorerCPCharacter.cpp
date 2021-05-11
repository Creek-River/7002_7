// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DungeonExplorerCPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" // ダメージ処理用に追加
#include "DE_GameInstance.h"// ゲームインスタンスに格納している残機情報取得のため追加

//////////////////////////////////////////////////////////////////////////
// ADungeonExplorerCPCharacter

ADungeonExplorerCPCharacter::ADungeonExplorerCPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // 600に変更	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	// 新たに追記
	CameraBoom->bDoCollisionTest = false; // カメラコリジョンテストは行わない
	CameraBoom->bInheritPitch = false; // Pitch制御なし
	CameraBoom->bInheritYaw = false; // Yaw制御なし
	CameraBoom->bInheritRoll = false; // Roll制御なし

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// プレイヤー挙動のカスタマイズ
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADungeonExplorerCPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADungeonExplorerCPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADungeonExplorerCPCharacter::MoveRight);

	// SuperSpeed関連のインプットイベントを登録
	PlayerInputComponent->BindAction("SuperSpeed", IE_Pressed, this, &ADungeonExplorerCPCharacter::OnSuperSpeedON);
	PlayerInputComponent->BindAction("SuperSpeed", IE_Released, this, &ADungeonExplorerCPCharacter::OnSuperSpeedOFF);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	// これらのキーバインドは使わないのでコメントアウト
	// PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("TurnRate", this, &ADungeonExplorerCPCharacter::TurnAtRate);
	// PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// PlayerInputComponent->BindAxis("LookUpRate", this, &ADungeonExplorerCPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADungeonExplorerCPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADungeonExplorerCPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADungeonExplorerCPCharacter::OnResetVR);
}

// ダメージ処理（ダメージを受けたときにコールされる）
float ADungeonExplorerCPCharacter::TakeDamage(float Damage, 
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, 
	class AActor* DamageCauser)
{
	// 親クラスのメソッドを実行しておく
	float damageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// 指定された爆発音を鳴らす
	UGameplayStatics::PlaySoundAtLocation(this, bangSound, GetActorLocation());
	// 指定された爆発パーティクルを現在地で発生させる
	// UWorld：発生させるワールド（通常はGetWorldで現在のワールドを取得）
	// UParticleSystem*：パーティクルアクタの参照
	// FTransform：発生場所のトランスフォーム情報
	// bAutoDestroy：終了時に自動消滅させるかどうか
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorTransform(), true);
	// カプセルコライダーを無効化する
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// キャラクタメッシュをDestroyする。Actorの場合はDestroy()だが、コンポーネント破壊はDestroyComponent()
	charMesh->DestroyComponent();

	// Game Instance取得
	UDE_GameInstance* DE_instance = Cast<UDE_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (DE_instance != NULL) {
		DE_instance->playerNum -= 1; // 残機 -1
		UE_LOG(LogTemp, Display, TEXT("PlayerNum : %d"), DE_instance->playerNum); // ログ出力しておく
		if (DE_instance->playerNum > 0) // 残機 0になったら
			// タイマーで1秒後にリスポーン関数を呼ぶ
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADungeonExplorerCPCharacter::Respawn, 1.0f, true);
	}

	return damageValue;
}

void ADungeonExplorerCPCharacter::Respawn()

{   // ここでは正式なリスポーンではなく、同じレベルを再読込する
	UGameplayStatics::OpenLevel(this, "DE_Level1", true, "");
}

// SuperSpeed関連
// インプットイベントからコールする。ここではフラグを上げ下げするだけ
void ADungeonExplorerCPCharacter::OnSuperSpeedON()
{
	isSuperSpeed = true;
}
void ADungeonExplorerCPCharacter::OnSuperSpeedOFF()
{
	isSuperSpeed = false;
}

// Tickイベント
void ADungeonExplorerCPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // 親クラスのメソッドを実行しておく
	if (isSuperSpeed) { // SuperSpeedモード中
		// ゲージを少しずつ消費する。DeltaTimeの乗算を忘れずに！
		// ゲージ値がはみ出ないよう、0.0-1.0の範囲でClampしておく
		superSpeedPower = FMath::Clamp(superSpeedPower - 0.3f*DeltaTime, 0.0f, 1.0f);
		// ゲージが残っている場合は歩行速度を倍加設定する
		if (superSpeedPower > 0.0f)
			GetCharacterMovement()->MaxWalkSpeed = 2.0f*walkSpeed;
		// ゲージ不足の場合は通常速度に設定
		else
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	else { // 通常モード
		// ゲージを少しずつ回復させる。DeltaTimeの乗算を忘れずに！
		superSpeedPower = FMath::Clamp(superSpeedPower + 0.05f*DeltaTime, 0.0f, 1.0f);
		// 通常速度に設定する
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	// ゲージ値確認のため、ログ出力しておく
	// UE_LOG(LogTemp, Display, TEXT("SuperSpeed Power : %f"),superSpeedPower);
}


void ADungeonExplorerCPCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADungeonExplorerCPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADungeonExplorerCPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADungeonExplorerCPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADungeonExplorerCPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADungeonExplorerCPCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		// プイレイヤー方向に依存させず、常にX方向に進むよう変更
		const FVector Direction = FVector(1.0f, 0.0f, 0.0f);
		AddMovementInput(Direction, Value);
	}
}

void ADungeonExplorerCPCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		// プイレイヤー方向に依存させず、常にY方向に進むよう変更
		const FVector Direction = FVector(0.0f, 1.0f, 0.0f);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
