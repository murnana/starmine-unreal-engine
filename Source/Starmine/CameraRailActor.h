#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraRailActor.generated.h"

class UCameraComponent;

// UCLASS() : UE のリフレクションシステムにこのクラスを登録するマクロ
UCLASS()
// 固定速度でスクロールする独立カメラアクター。縦スクロール STG の視点を担当する
class STARMINE_API ACameraRailActor : public AActor
{
	GENERATED_BODY()

public:
	ACameraRailActor();

protected:
	virtual void Tick(float DeltaTime) override;

private:
	// 真上から見下ろす平行投影カメラ
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// 1秒あたりのスクロール速度（cm/s）。エディタ・Blueprint から調整できる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", meta=(AllowPrivateAccess=true))
	float ScrollSpeed = 100.0f;
};
