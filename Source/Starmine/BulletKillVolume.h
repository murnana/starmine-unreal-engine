#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletKillVolume.generated.h"

class UBoxComponent;

UCLASS()
// 画面外に配置する「弾消去ボリューム」。このボリュームに重なった弾は自動破棄される
class STARMINE_API ABulletKillVolume : public AActor
{
	GENERATED_BODY()

public:
	ABulletKillVolume();

private:
	// オーバーラップ専用の矩形コリジョン。物理ブロックは行わない
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> KillZone;
};
