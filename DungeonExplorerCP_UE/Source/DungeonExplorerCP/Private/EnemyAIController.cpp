// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCPCharacter.h" //�G�l�~�[�L�����N�^��������ŕK�v

//�R���X�g���N�^
AEnemyAIController::AEnemyAIController(const class FObjectInitializer& PCIP)
	:Super(PCIP)
{
	//BlackBoard�R���|�[�l���g���쐬
	bb = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Black Board"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//waypoint�擾�ׁ̈AEnemyCharacter���擾
	AEnemyCPCharacter* enemyCharacter = Cast<AEnemyCPCharacter>(GetPawn());

	if (bt != NULL)
		RunBehaviorTree(bt); //BT���N��
	if (bb != NULL) {
		//waypoint���擾�@&& BB�L�[�l�Ƃ��ēo�^
		for (int i = 0; i < enemyCharacter->waypointTargets.Num(); i++) {
			FString key = "waypoint" + FString::FromInt(i + 1);
			bb->SetValueAsObject(*key, enemyCharacter->waypointTargets[i]);
		}
	}

	//Pawn Sensor�̃p�����[�^�ݒ�
	PawnSensor->SightRadius = 900.0f; //���씼�a
	PawnSensor->SensingInterval = 0.5f; //�m�o�̍X�V�p�x
	PawnSensor->bEnableSensingUpdates = true;
	PawnSensor->bOnlySensePlayers = false; 
	PawnSensor->SetPeripheralVisionAngle(60.0f); //����̍L��

	//Delegate��o�^
	FScriptDelegate OnSeePawnDelegate;
	OnSeePawnDelegate.BindUFunction(this, "OnSeePawn");
	PawnSensor->OnSeePawn.Add(OnSeePawnDelegate);
	FScriptDelegate OnHearNoiseDelegate;
	OnHearNoiseDelegate.BindUFunction(this, "OnHearNoise");
	PawnSensor->OnHearNoise.Add(OnHearNoiseDelegate);
}

void AEnemyAIController::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	
}

void AEnemyAIController::OnSeePawn(APawn* OtherPawn)
{
	FString message = TEXT("Saw Pawn") + OtherPawn->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, message);

	bb->SetValueAsObject("Player", OtherPawn); //Player�L�[�l�����
	//�R�[���o�b�N�̃^�C�}�[����
	GetWorld()->GetTimerManager().SetTimer(timerHandle, 
		this, &AEnemyAIController::OnChaseTimeOut, 3.0f, true);

}


void AEnemyAIController::OnChaseTimeOut()
{
	//�L�[�̒l��NULL�ɐݒ肵�ď���s���ɖ߂�
	bb->SetValueAsObject("Player", NULL);
	//�f�o�b�O�o��
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Timeout");
}
