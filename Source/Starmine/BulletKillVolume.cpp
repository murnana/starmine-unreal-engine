#include "BulletKillVolume.h"
#include "Components/BoxComponent.h"

ABulletKillVolume::ABulletKillVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	KillZone = CreateDefaultSubobject<UBoxComponent>(TEXT("KillZone"));
	// 初期サイズは 2000x2000x2000 cm。エディタで自由にリサイズして使う
	KillZone->SetBoxExtent(FVector(2000.0f, 2000.0f, 2000.0f));
	// OverlapAll：すべてのオブジェクトタイプにオーバーラップ応答。物理ブロックは行わない
	KillZone->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = KillZone;
}
