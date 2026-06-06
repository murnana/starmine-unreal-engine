#include "CameraRailActor.h"
// UCameraComponent の定義
#include "Camera/CameraComponent.h"

ACameraRailActor::ACameraRailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;
	// Z=1000cm 上空に配置し、真下（Pitch=-90）を向かせる
	Camera->SetRelativeLocationAndRotation(FVector(0.0, 0.0, 1000.0), FRotator(-90.0, 0.0, 0.0));
	// 平行投影モード：遠近感をなくし 2D ゲームらしく見せる
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->OrthoWidth = 1024.0f;
}

void ACameraRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorWorldOffset(FVector(ScrollSpeed * DeltaTime, 0.0, 0.0));
}
