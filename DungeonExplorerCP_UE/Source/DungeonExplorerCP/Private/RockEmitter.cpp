// Fill out your copyright notice in the Description page of Project Settings.

#include "RockEmitter.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h" // AStaticMeshActorを扱うために必要
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" // FClassFinderのために必要
#include "Runtime/Engine/Classes/Engine/World.h" // FActorSpawnParameterなどを扱うために必要

// Sets default values
ARockEmitter::ARockEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BP_rockスタティックメッシュアクタを探して変数格納
	static ConstructorHelpers::FClassFinder<AStaticMeshActor> smActor(TEXT("/Game/DungeonExplorer/GraphicAssets/BP_rock.BP_rock_C"));
	rockActor = smActor.Class;
}

// Called when the game starts or when spawned
void ARockEmitter::BeginPlay()
{
	Super::BeginPlay();
	
	// タイマーを起動して"SpawnRock()メソッドを呼ぶ"
	// 下記のようにデリゲートメソッドとして指定する
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARockEmitter::SpawnRock, delayTime, true);
}

void ARockEmitter::SpawnRock() {
	FActorSpawnParameters params; // アクタをスポーンするときに必要

	/*　本来は次のようにパラメータ値を設定するが、デフォルト値のままでもOK
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

	// 出現位置のズレ・角度ズレ分をランダム値で算出
	FVector pos = FVector(FMath::RandRange(-1000, 1000), FMath::RandRange(-1000, 1000), 0.0f);
	FRotator rot = FRotator(FMath::RandRange(-180, 180), FMath::RandRange(-180, 180), FMath::RandRange(-180, 180));

	// 算出値をもとに岩をスポーンさせる
	GetWorld()->SpawnActor<AStaticMeshActor>(rockActor, GetActorLocation() + pos, GetActorRotation() + rot, params);

	// 再帰的に自身を呼び出すべくタイマーセットする
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARockEmitter::SpawnRock, delayTime, true);
}

// Called every frame
void ARockEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

