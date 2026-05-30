#include "StarmineGameMode.h"
// APlayerShipPawn の定義。DefaultPawnClass に設定するために必要
#include "PlayerShipPawn.h"

AStarmineGameMode::AStarmineGameMode()
{
	// ゲーム開始時にプレイヤーへ自動的にポーズされる Pawn のクラスを指定する。
	// これにより PlayerShipPawn がスポーンされ、プレイヤーが操作できるようになる
	DefaultPawnClass = APlayerShipPawn::StaticClass();
}
