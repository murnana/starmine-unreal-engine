#include "PlayerShipPawn.h"
#include "BulletActor.h"
// USphereComponent の定義。球体の当たり判定コンポーネント
#include "Components/SphereComponent.h"
// UDynamicMeshComponent の定義。実行時に頂点・三角形を動的に変更できるメッシュコンポーネント
#include "Components/DynamicMeshComponent.h"
// FDynamicMesh3 の定義。頂点リストと三角形リストを持つジオメトリデータ構造
#include "DynamicMesh/DynamicMesh3.h"
// UEnhancedInputComponent の定義。InputAction とハンドラ関数を紐付けるコンポーネント
#include "EnhancedInputComponent.h"
// UEnhancedInputLocalPlayerSubsystem の定義。InputMappingContext を登録・解除するサブシステム
#include "EnhancedInputSubsystems.h"
// UFloatingPawnMovement の定義。重力なしでシンプルな移動を提供するコンポーネント
#include "GameFramework/FloatingPawnMovement.h"
// UInputAction の定義。"Move" や "Fire" などの抽象的な入力アクション
#include "InputAction.h"
// UInputMappingContext の定義。物理キーと InputAction の対応表
#include "InputMappingContext.h"

APlayerShipPawn::APlayerShipPawn()
{
	// Tick（毎フレーム更新）は不要なので無効にする
	PrimaryActorTick.bCanEverTick = false;

	// CreateDefaultSubobject：コンストラクタ専用の Component 生成関数
	// 生成した Component は自動的に GC 管理下に置かれる
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(15.0f);
	// このアクターの RootComponent（位置・回転・スケールの基準）に設定する
	RootComponent = CollisionSphere;

	// 船体メッシュを CollisionSphere の子として追加する
	ShipMesh = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(CollisionSphere);

	// 重力なしで XY 平面上を自由に移動できるコンポーネントを追加する
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	// 最大移動速度を 600 cm/s に設定する
	MovementComponent->MaxSpeed = 600.0f;
	// Z 軸方向への移動を禁止する（2D シューティングのため奥行き移動は不要）
	MovementComponent->SetPlaneConstraintEnabled(true);
	MovementComponent->SetPlaneConstraintNormal(FVector(0.0, 0.0, 1.0));
}

void APlayerShipPawn::OnConstruction(const FTransform& Transform)
{
	// 親クラスの OnConstruction を必ず最初に呼ぶ（UE の規約）
	Super::OnConstruction(Transform);

	// FDynamicMesh3：頂点リストと三角形リストを持つシンプルなジオメトリデータ構造
	UE::Geometry::FDynamicMesh3 Mesh;
	// 頂点カラーを有効化する（色はオーバーレイとして三角形の角ごとに持つ）
	Mesh.EnableAttributes();
	Mesh.Attributes()->EnablePrimaryColors();
	UE::Geometry::FDynamicMeshColorOverlay* ColorOverlay = Mesh.Attributes()->PrimaryColors();

	// 単位は cm。XY 平面（Z=0）上に船体の三角形を定義する。+Y 方向が機首（前方）
	// 頂点順序（反時計回り）で法線が +Z（カメラ側）になる
	auto index1 = Mesh.AppendVertex(FVector3d(-50.0, -50.0, 0.0));
	auto index2 = Mesh.AppendVertex(FVector3d(-50.0, 50.0, 0.0));
	auto index3 = Mesh.AppendVertex(FVector3d(50.0, 0.0, 0.0));
	// 3つの頂点インデックスで三角形を1枚定義する（戻り値は三角形の ID）
	auto triangleId = Mesh.AppendTriangle(index1, index2, index3);

	// 左右の頂点はグレー、機首（先端）は白のグラデーション
	auto colorIndex1 = ColorOverlay->AppendElement(FVector4f(0.5f, 0.5f, 0.5f, 1.0f));
	auto colorIndex2 = ColorOverlay->AppendElement(FVector4f(0.5f, 0.5f, 0.5f, 1.0f));
	auto colorIndex3 = ColorOverlay->AppendElement(FVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	// 三角形の3つの角にそれぞれの色要素を割り当てる
	ColorOverlay->SetTriangle(triangleId, UE::Geometry::FIndex3i(colorIndex1, colorIndex2, colorIndex3));

	// MoveTemp：所有権を移してコピーを避ける（Mesh はここで空になる）
	ShipMesh->SetMesh(MoveTemp(Mesh));
	// マテリアルなしで頂点カラーをそのまま描画するモードに設定する
	ShipMesh->SetColorOverrideMode(EDynamicMeshComponentColorOverrideMode::VertexColors);
}

void APlayerShipPawn::BeginPlay()
{
	// 親クラスの BeginPlay を必ず最初に呼ぶ（UE の規約）
	Super::BeginPlay();

	// このポーンを操作しているプレイヤーコントローラーを取得する
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Enhanced Input のサブシステムを取得する。
		// ローカルプレイヤーごとに独立したサブシステムが存在する
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// InputMappingContext を登録する。
			// 優先度 0 は最も低い優先度で、複数の MappingContext が競合した際に使われる
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void APlayerShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 親クラスの SetupPlayerInputComponent を呼ぶ（UE の規約）
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// UEnhancedInputComponent にキャストする。
	// Enhanced Input が有効でない場合は nullptr になるため早期リターンする
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC)
	{
		return;
	}

	// MoveAction が設定されていれば、入力中（Triggered）に OnMove を呼ぶよう登録する
	if (MoveAction)
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShipPawn::OnMove);
	}
	// FireAction が設定されていれば、入力中（Triggered）に OnFire を呼ぶよう登録する
	if (FireAction)
	{
		EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerShipPawn::OnFire);
	}
}

void APlayerShipPawn::OnMove(const FInputActionValue& Value)
{
	// コントローラーまたはキーボード (WASD) から、入力された向きを取得
	// その入力を伝えます
	const FVector2D Input = Value.Get<FVector2D>();
	AddMovementInput(FVector(Input.X, Input.Y, 0.0));
}

void APlayerShipPawn::OnFire(const FInputActionValue& Value)
{
	if (!BulletClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 自機の前方 60cm にスポーンする（自機メッシュと重ならないようにオフセット）
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 60.0f;
	World->SpawnActor<ABulletActor>(BulletClass, SpawnLocation, GetActorRotation());
}
