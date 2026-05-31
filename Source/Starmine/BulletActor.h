#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

class USphereComponent;
class UDynamicMeshComponent;
class UProjectileMovementComponent;

UCLASS()
// プレイヤーが発射する弾丸アクター。+X 方向へ直進し、画面外で自動破棄される
class STARMINE_API ABulletActor : public AActor
{
	GENERATED_BODY()

public:
	ABulletActor();

protected:
	virtual void BeginPlay() override;
	// 弾丸の見た目（頂点メッシュ）を構築する
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	// ABulletKillVolume に重なった瞬間に呼ばれるコールバック
	// UFUNCTION() は AddDynamic で登録するために必須
	UFUNCTION()
	void OnHitKillVolume(AActor* OverlappedActor, AActor* OtherActor);

private:
	// Details パネルで弾の色を指定できる。OnConstruction で参照されメッシュに反映される
	UPROPERTY(EditAnywhere, Category="Bullet")
	FLinearColor BulletColor = FLinearColor::White;

	// 当たり判定の球体コリジョン。Root として位置の基準になる
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	// 弾丸の見た目を表す動的メッシュ
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDynamicMeshComponent> BulletMesh;

	// 直線移動を担当するコンポーネント。重力なしで +X 方向へ飛ぶ
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
};
