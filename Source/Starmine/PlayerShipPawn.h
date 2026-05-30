#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShipPawn.generated.h"

class UBoxComponent;
class UDynamicMeshComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class STARMINE_API APlayerShipPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerShipPawn();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDynamicMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	// エディタの Details パネルから InputAction アセットを割り当てる
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	void OnMove(const FInputActionValue& Value);
	void OnFire(const FInputActionValue& Value);
};
