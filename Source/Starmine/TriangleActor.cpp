#include "TriangleActor.h"
// FDynamicMesh3 の定義。GeometryCore モジュールが提供する純粋な C++ ジオメトリデータ構造
#include "DynamicMesh/DynamicMesh3.h"

ATriangleActor::ATriangleActor()
{
	// Tick（毎フレーム更新）は不要なので無効にする
	PrimaryActorTick.bCanEverTick = false;

	// CreateDefaultSubobject：コンストラクタ専用の Component 生成関数
	// 生成した Component は自動的に GC 管理下に置かれる
	MeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("MeshComponent"));
	// このアクターの RootComponent（位置・回転・スケールの基準）に設定する
	RootComponent = MeshComponent;
}

void ATriangleActor::OnConstruction(const FTransform& Transform)
{
	// 親クラスの OnConstruction を必ず最初に呼ぶ（UE の規約）
	Super::OnConstruction(Transform);

	// FDynamicMesh3：頂点リストと三角形リストを持つシンプルなジオメトリデータ構造
	UE::Geometry::FDynamicMesh3 Mesh;

	// 単位は cm。XY 平面上に 50cm スケールの三角形を定義する
	// 頂点順序（反時計回り）で法線が +Z（上向き）になる
	int32 index1 = Mesh.AppendVertex(FVector3d(50.0, 0.0, 0.0));
	int32 index2 = Mesh.AppendVertex(FVector3d(-50.0, 0.0, 0.0));
	int32 index3 = Mesh.AppendVertex(FVector3d(0.0, 100.0, 0.0));

	// 3つの頂点インデックスで三角形を1枚定義する（戻り値は三角形のID）
	int32 TriangleId = Mesh.AppendTriangle(index2, index1, index3);

	// 頂点カラーを有効化する（色はオーバーレイとして三角形の角ごとに持つ）
	Mesh.EnableAttributes();
	Mesh.Attributes()->EnablePrimaryColors();
	UE::Geometry::FDynamicMeshColorOverlay* ColorOverlay = Mesh.Attributes()->PrimaryColors();

	int32 c0 = ColorOverlay->AppendElement(FVector4f(1.0f, 0.0f, 0.0f, 1.0f));
	int32 c1 = ColorOverlay->AppendElement(FVector4f(0.0f, 1.0f, 0.0f, 1.0f));
	int32 c2 = ColorOverlay->AppendElement(FVector4f(0.0f, 0.0f, 1.0f, 1.0f));

	// 三角形の3つの角にそれぞれの色要素を割り当てる
	ColorOverlay->SetTriangle(TriangleId, UE::Geometry::FIndex3i(c0, c1, c2));

	// MoveTemp：所有権を移してコピーを避ける（Mesh はここで空になる）
	MeshComponent->SetMesh(MoveTemp(Mesh));

	// マテリアルなしで頂点カラーをそのまま描画するモードに設定する
	MeshComponent->SetColorOverrideMode(EDynamicMeshComponentColorOverrideMode::VertexColors);
}

void ATriangleActor::BeginPlay()
{
	// 親クラスの BeginPlay を必ず最初に呼ぶ（UE の規約）
	Super::BeginPlay();
}
