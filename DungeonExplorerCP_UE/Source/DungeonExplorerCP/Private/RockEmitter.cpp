// Fill out your copyright notice in the Description page of Project Settings.

#include "RockEmitter.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h" // AStaticMeshActor���������߂ɕK�v
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" // FClassFinder�̂��߂ɕK�v
#include "Runtime/Engine/Classes/Engine/World.h" // FActorSpawnParameter�Ȃǂ��������߂ɕK�v

// Sets default values
ARockEmitter::ARockEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BP_rock�X�^�e�B�b�N���b�V���A�N�^��T���ĕϐ��i�[
	static ConstructorHelpers::FClassFinder<AStaticMeshActor> smActor(TEXT("/Game/DungeonExplorer/GraphicAssets/BP_rock.BP_rock_C"));
	rockActor = smActor.Class;
}

// Called when the game starts or when spawned
void ARockEmitter::BeginPlay()
{
	Super::BeginPlay();
	
	// �^�C�}�[���N������"SpawnRock()���\�b�h���Ă�"
	// ���L�̂悤�Ƀf���Q�[�g���\�b�h�Ƃ��Ďw�肷��
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARockEmitter::SpawnRock, delayTime, true);
}

void ARockEmitter::SpawnRock() {
	FActorSpawnParameters params; // �A�N�^���X�|�[������Ƃ��ɕK�v

	/*�@�{���͎��̂悤�Ƀp�����[�^�l��ݒ肷�邪�A�f�t�H���g�l�̂܂܂ł�OK
	params.bAllowDuringConstructionScript = true;
	params.bDeferConstruction = false;
	params.bNoFail = true;
	params.Name = {};
	params.ObjectFlags = EObjectFlags::RF_NoFlags;
	params.OverrideLevel = nullptr;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	params.Template = nullptr;
	*/

	// �o���ʒu�̃Y���E�p�x�Y�����������_���l�ŎZ�o
	FVector pos = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 0.0f);
	FRotator rot = FRotator(FMath::RandRange(-180, 180), FMath::RandRange(-180, 180), FMath::RandRange(-180, 180));

	// �Z�o�l�����ƂɊ���X�|�[��������
	GetWorld()->SpawnActor<AStaticMeshActor>(rockActor, GetActorLocation() + pos, GetActorRotation() + rot, params);

	// �ċA�I�Ɏ��g���Ăяo���ׂ��^�C�}�[�Z�b�g����
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARockEmitter::SpawnRock, delayTime, true);
}

// Called every frame
void ARockEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

