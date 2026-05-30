#pragma once

// UE の基本型（FVector, FString など）をまとめてインクルードする
#include "CoreMinimal.h"
// AGameModeBase の定義。ゲームのルール（使用する Pawn・HUD・PlayerController など）を管理する基底クラス
#include "GameFramework/GameModeBase.h"
// UHT が自動生成するファイル。必ず最後に置く
#include "StarmineGameMode.generated.h"

// UCLASS() : UE のリフレクションシステムにこのクラスを登録するマクロ
UCLASS()
// Starmine ゲーム全体のルールを定義する GameMode。
// デフォルトの Pawn クラスとして APlayerShipPawn を指定する
class STARMINE_API AStarmineGameMode : public AGameModeBase
{
	// GENERATED_BODY() : UHT が生成したコード（GC 連携・シリアライズ等）の挿入点
	GENERATED_BODY()

public:
	// コンストラクタ：DefaultPawnClass に APlayerShipPawn を設定する
	AStarmineGameMode();
};
