#include "CameraRailActor.h"
#include "Camera/CameraComponent.h"
#include "BulletKillVolume.h"
#include "Engine/Engine.h"

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

FBox2D ACameraRailActor::GetViewBounds2D() const
{
	// フォールバック用アスペクト比。Viewport が取得できない場合に使う
	float AspectRatio = 16.0f / 9.0f;
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		if (ViewportSize.Y > 0.0f)
		{
			AspectRatio = ViewportSize.X / ViewportSize.Y;
		}
	}

	// カメラ Pitch=-90（真下向き）のとき、OrthoWidth は世界空間 Y 軸に対応する
	const float HalfY = Camera->OrthoWidth * 0.5f;
	const float HalfX = HalfY / AspectRatio;
	const FVector Loc = GetActorLocation();

	return FBox2D(
		FVector2D(Loc.X - HalfX, Loc.Y - HalfY),
		FVector2D(Loc.X + HalfX, Loc.Y + HalfY)
	);
}

void ACameraRailActor::BeginPlay()
{
	Super::BeginPlay();

	auto world = GetWorld();
	this->KillVolume = world->SpawnActor<ABulletKillVolume>();
	this->KillVolume->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ACameraRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorWorldOffset(FVector(ScrollSpeed * DeltaTime, 0.0, 0.0));
}
