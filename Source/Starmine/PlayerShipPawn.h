#pragma once

// UE の基本型（FVector, FString など）をまとめてインクルードする
#include "CoreMinimal.h"
// APawn の定義。プレイヤーが操作できるアクターの基底クラス
#include "GameFramework/Pawn.h"
// UHT が自動生成するファイル。必ず最後に置く
#include "PlayerShipPawn.generated.h"

// 前方宣言：型が存在することだけ伝え、ヘッダのインクルードを減らしてコンパイルを高速化する
class UBoxComponent;
class UCameraComponent;
class UDynamicMeshComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// UCLASS() : UE のリフレクションシステムにこのクラスを登録するマクロ
UCLASS()
// プレイヤーが操作する自機 Pawn。XY 平面上を移動し、弾を発射する
// STARMINE_API : このクラスを DLL エクスポート対象にするマクロ（他モジュールから参照できるようにする）
class STARMINE_API APlayerShipPawn : public APawn
{
	// GENERATED_BODY() : UHT が生成したコード（GC 連携・シリアライズ等）の挿入点
	GENERATED_BODY()

public:
	// コンストラクタ：Component の生成と初期パラメータの設定を行う
	APlayerShipPawn();

protected:
	// OnConstruction：エディタ上で配置・移動するたびに呼ばれる。船体メッシュを再生成する
	virtual void OnConstruction(const FTransform& Transform) override;
	// BeginPlay：ゲーム開始時に呼ばれる。Enhanced Input の MappingContext を登録する
	virtual void BeginPlay() override;
	// SetupPlayerInputComponent：入力アクションとハンドラ関数を紐付ける
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// 真上から見下ろす平行投影カメラ。Z 軸上方に固定配置し、Pitch=-90 で XY 平面を映す
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// 当たり判定の矩形コリジョン。Root として位置・回転の基準になる
	// UPROPERTY(VisibleAnywhere) : エディタの Details パネルで確認できる（編集はできない）
	// TObjectPtr<T> : UE5 推奨の UPROPERTY 用ポインタ型。GC がこのポインタを追跡する
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	// 船体の三角形メッシュを描画するコンポーネント
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDynamicMeshComponent> ShipMesh;

	// XY 平面上の移動を担当するコンポーネント。Z 方向への移動は制限している
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	// エディタの Details パネルから InputMappingContext アセットを割り当てる。
	// どのキーをどのアクションに対応させるか定義したマッピング表
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	// 移動入力（WASD など）に対応する InputAction アセット
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	// 発射入力（スペースキーなど）に対応する InputAction アセット
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> FireAction;

	// MoveAction が入力されたときに呼ばれる。入力値をもとに XY 平面を移動する
	void OnMove(const FInputActionValue& Value);
	// FireAction が入力されたときに呼ばれる。弾丸を生成する（未実装）
	void OnFire(const FInputActionValue& Value);
};
