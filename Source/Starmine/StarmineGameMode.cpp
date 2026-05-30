#include "StarmineGameMode.h"
#include "PlayerShipPawn.h"

AStarmineGameMode::AStarmineGameMode()
{
	DefaultPawnClass = APlayerShipPawn::StaticClass();
}
