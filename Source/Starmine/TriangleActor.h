#pragma once

// UE の基本型（FVector, FString など）をまとめてインクルードする
#include "CoreMinimal.h"
// UDynamicMeshComponent の定義。GeometryFramework モジュールが提供する
#include "Components/DynamicMeshComponent.h"
// このクラスの親クラス AActor の定義
#include "GameFramework/Actor.h"
// Unreal Header Tool (UHT) が自動生成するファイル。必ず最後に置く
#include "TriangleActor.generated.h"

// UCLASS() : UE のリフレクションシステムにこのクラスを登録するマクロ
UCLASS()
// STARMINE_API : このクラスを DLL エクスポート対象にするマクロ（他モジュールから参照できるようにする）
class STARMINE_API ATriangleActor : public AActor
{
	// GENERATED_BODY() : UHT が生成したコード（GC 連携・シリアライズ等）の挿入点
	GENERATED_BODY()

public:
	// コンストラクタ：Component の生成など、ゲーム開始前の初期化を行う
	ATriangleActor();

protected:
	// OnConstruction：エディタ上でアクターを配置・移動するたびに呼ばれる。エディタでも確認できる
	virtual void OnConstruction(const FTransform& Transform) override;

	// BeginPlay：アクターがゲームワールドに配置されて最初のフレームが始まるときに呼ばれる
	virtual void BeginPlay() override;

private:
	// UPROPERTY(VisibleAnywhere) : エディタの Details パネルで確認できる（編集はできない）
	// TObjectPtr<T> : UE5 推奨の UPROPERTY 用ポインタ型。GC がこのポインタを追跡する
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDynamicMeshComponent> MeshComponent;
};
