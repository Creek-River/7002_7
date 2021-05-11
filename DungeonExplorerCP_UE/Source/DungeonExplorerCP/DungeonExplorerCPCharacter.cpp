// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DungeonExplorerCPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" // �_���[�W�����p�ɒǉ�
#include "DE_GameInstance.h"// �Q�[���C���X�^���X�Ɋi�[���Ă���c�@���擾�̂��ߒǉ�

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
	CameraBoom->TargetArmLength = 600.0f; // 600�ɕύX	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	// �V���ɒǋL
	CameraBoom->bDoCollisionTest = false; // �J�����R���W�����e�X�g�͍s��Ȃ�
	CameraBoom->bInheritPitch = false; // Pitch����Ȃ�
	CameraBoom->bInheritYaw = false; // Yaw����Ȃ�
	CameraBoom->bInheritRoll = false; // Roll����Ȃ�

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// �v���C���[�����̃J�X�^�}�C�Y
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

	// SuperSpeed�֘A�̃C���v�b�g�C�x���g��o�^
	PlayerInputComponent->BindAction("SuperSpeed", IE_Pressed, this, &ADungeonExplorerCPCharacter::OnSuperSpeedON);
	PlayerInputComponent->BindAction("SuperSpeed", IE_Released, this, &ADungeonExplorerCPCharacter::OnSuperSpeedOFF);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	// �����̃L�[�o�C���h�͎g��Ȃ��̂ŃR�����g�A�E�g
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

// �_���[�W�����i�_���[�W���󂯂��Ƃ��ɃR�[�������j
float ADungeonExplorerCPCharacter::TakeDamage(float Damage, 
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, 
	class AActor* DamageCauser)
{
	// �e�N���X�̃��\�b�h�����s���Ă���
	float damageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// �w�肳�ꂽ��������炷
	UGameplayStatics::PlaySoundAtLocation(this, bangSound, GetActorLocation());
	// �w�肳�ꂽ�����p�[�e�B�N�������ݒn�Ŕ���������
	// UWorld�F���������郏�[���h�i�ʏ��GetWorld�Ō��݂̃��[���h���擾�j
	// UParticleSystem*�F�p�[�e�B�N���A�N�^�̎Q��
	// FTransform�F�����ꏊ�̃g�����X�t�H�[�����
	// bAutoDestroy�F�I�����Ɏ������ł����邩�ǂ���
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, GetActorTransform(), true);
	// �J�v�Z���R���C�_�[�𖳌�������
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �L�����N�^���b�V����Destroy����BActor�̏ꍇ��Destroy()�����A�R���|�[�l���g�j���DestroyComponent()
	charMesh->DestroyComponent();

	// Game Instance�擾
	UDE_GameInstance* DE_instance = Cast<UDE_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (DE_instance != NULL) {
		DE_instance->playerNum -= 1; // �c�@ -1
		UE_LOG(LogTemp, Display, TEXT("PlayerNum : %d"), DE_instance->playerNum); // ���O�o�͂��Ă���
		if (DE_instance->playerNum > 0) // �c�@ 0�ɂȂ�����
			// �^�C�}�[��1�b��Ƀ��X�|�[���֐����Ă�
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ADungeonExplorerCPCharacter::Respawn, 1.0f, true);
	}

	return damageValue;
}

void ADungeonExplorerCPCharacter::Respawn()

{   // �����ł͐����ȃ��X�|�[���ł͂Ȃ��A�������x�����ēǍ�����
	UGameplayStatics::OpenLevel(this, "DE_Level1", true, "");
}

// SuperSpeed�֘A
// �C���v�b�g�C�x���g����R�[������B�����ł̓t���O���グ�������邾��
void ADungeonExplorerCPCharacter::OnSuperSpeedON()
{
	isSuperSpeed = true;
}
void ADungeonExplorerCPCharacter::OnSuperSpeedOFF()
{
	isSuperSpeed = false;
}

// Tick�C�x���g
void ADungeonExplorerCPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // �e�N���X�̃��\�b�h�����s���Ă���
	if (isSuperSpeed) { // SuperSpeed���[�h��
		// �Q�[�W�������������BDeltaTime�̏�Z��Y�ꂸ�ɁI
		// �Q�[�W�l���͂ݏo�Ȃ��悤�A0.0-1.0�͈̔͂�Clamp���Ă���
		superSpeedPower = FMath::Clamp(superSpeedPower - 0.3f*DeltaTime, 0.0f, 1.0f);
		// �Q�[�W���c���Ă���ꍇ�͕��s���x��{���ݒ肷��
		if (superSpeedPower > 0.0f)
			GetCharacterMovement()->MaxWalkSpeed = 2.0f*walkSpeed;
		// �Q�[�W�s���̏ꍇ�͒ʏ푬�x�ɐݒ�
		else
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	else { // �ʏ탂�[�h
		// �Q�[�W���������񕜂�����BDeltaTime�̏�Z��Y�ꂸ�ɁI
		superSpeedPower = FMath::Clamp(superSpeedPower + 0.05f*DeltaTime, 0.0f, 1.0f);
		// �ʏ푬�x�ɐݒ肷��
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	// �Q�[�W�l�m�F�̂��߁A���O�o�͂��Ă���
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
		// �v�C���C���[�����Ɉˑ��������A���X�����ɐi�ނ悤�ύX
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
		// �v�C���C���[�����Ɉˑ��������A���Y�����ɐi�ނ悤�ύX
		const FVector Direction = FVector(0.0f, 1.0f, 0.0f);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
