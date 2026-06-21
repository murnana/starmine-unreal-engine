#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraRailActor.generated.h"

class UCameraComponent;
class ABulletKillVolume;

// UCLASS() : UE のリフレクションシステムにこのクラスを登録するマクロ
UCLASS()
// 固定速度でスクロールする独立カメラアクター。縦スクロール STG の視点を担当する
class STARMINE_API ACameraRailActor : public AActor
{
	GENERATED_BODY()

public:
	ACameraRailActor();

	// カメラが映す XY 平面上の矩形範囲を返す（プレイヤー移動制限に使用）
	FBox2D GetViewBounds2D() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 真上から見下ろす平行投影カメラ
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// カメラに追従する弾消去ボリューム。BeginPlay でスポーン・アタッチされる
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABulletKillVolume> KillVolume;

	// 1秒あたりのスクロール速度（cm/s）。エディタ・Blueprint から調整できる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(AllowPrivateAccess=true))
	float ScrollSpeed = 100.0f;
};
