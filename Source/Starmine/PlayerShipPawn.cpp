#include "PlayerShipPawn.h"
#include "Components/BoxComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "InputMappingContext.h"

APlayerShipPawn::APlayerShipPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(20.0, 20.0, 5.0));
	RootComponent = CollisionBox;

	ShipMesh = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(CollisionBox);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 600.0f;
	MovementComponent->SetPlaneConstraintEnabled(true);
	MovementComponent->SetPlaneConstraintNormal(FVector(0.0, 0.0, 1.0));
}

void APlayerShipPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE::Geometry::FDynamicMesh3 Mesh;
	Mesh.EnableAttributes();
	Mesh.Attributes()->EnablePrimaryColors();
	UE::Geometry::FDynamicMeshColorOverlay* ColorOverlay = Mesh.Attributes()->PrimaryColors();

	// TODO(human): 船体の頂点・三角形・頂点カラーをここに追加する
	// 座標系: XY 平面（Z=0）、+Y 方向が機首（前方）
	// 使用する API:
	auto index1 = Mesh.AppendVertex(FVector3d(-50.0, -50.0, 0.0));
	auto index2 = Mesh.AppendVertex(FVector3d(50.0, -50.0, 0.0));
	auto index3 = Mesh.AppendVertex(FVector3d(0.0, 50.0, 0.0));
	auto triangleId = Mesh.AppendTriangle(index1, index2, index3);

	auto colorIndex1 = ColorOverlay->AppendElement(FVector4f(0.5f, 0.5f, 0.5f, 1.0f));
	auto colorIndex2 = ColorOverlay->AppendElement(FVector4f(0.5f, 0.5f, 0.5f, 1.0f));
	auto colorIndex3 = ColorOverlay->AppendElement(FVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	ColorOverlay->SetTriangle(triangleId, UE::Geometry::FIndex3i(colorIndex1, colorIndex2, colorIndex3));

	ShipMesh->SetMesh(MoveTemp(Mesh));
	ShipMesh->SetColorOverrideMode(EDynamicMeshComponentColorOverrideMode::VertexColors);
}

void APlayerShipPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		return;
	}

	if (MoveAction)
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShipPawn::OnMove);
	}
	if (FireAction)
	{
		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerShipPawn::OnFire);
	}
}

void APlayerShipPawn::OnMove(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	AddMovementInput(FVector(Input.X, Input.Y, 0.0));
}

void APlayerShipPawn::OnFire(const FInputActionValue& Value)
{
	// 弾丸生成は後のタスクで実装する
}
