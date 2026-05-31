#include "BulletActor.h"
#include "BulletKillVolume.h"
#include "Components/SphereComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABulletActor::ABulletActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	// 弾丸の当たり判定半径 8cm
	CollisionSphere->SetSphereRadius(8.0f);
	RootComponent = CollisionSphere;

	BulletMesh = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(CollisionSphere);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	// スポーン時に Actor の Forward ベクトル(+X)× InitialSpeed が初速として自動設定される
	MovementComponent->InitialSpeed = 1200.0f;
	MovementComponent->MaxSpeed = 1200.0f;
	// 宇宙空間なので重力なし
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	// OnActorEndOverlap：自分が別アクターから分離した瞬間に発火する
	// ボリューム外に出たら Destroy するため、EndOverlap を使う
	OnActorEndOverlap.AddDynamic(this, &ABulletActor::OnHitKillVolume);
}

void ABulletActor::OnHitKillVolume(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor)
	{
		return;
	}

	if (OtherActor->IsA<ABulletKillVolume>())
	{
		Destroy();
	}
}

void ABulletActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE::Geometry::FDynamicMesh3 Mesh;
	Mesh.EnableAttributes();
	Mesh.Attributes()->EnablePrimaryColors();
	UE::Geometry::FDynamicMeshColorOverlay* ColorOverlay = Mesh.Attributes()->PrimaryColors();

	constexpr int CircumferencePointCount = 8;
	int Indexes[CircumferencePointCount + 1];
	int ColorIndexes[CircumferencePointCount + 1];

	const FVector4f Color(BulletColor.R, BulletColor.G, BulletColor.B, BulletColor.A);

	Indexes[0] = Mesh.AppendVertex(FVector3d(0, 0, 0));
	ColorIndexes[0] = ColorOverlay->AppendElement(Color);

	// 円周を CircumferencePointCount 等分した角度で頂点を配置する
	for (int i = 0; i < CircumferencePointCount; ++i)
	{
		constexpr double Length = 40.0;
		const double Angle = (2.0 * PI * i) / CircumferencePointCount;
		Indexes[i + 1] = Mesh.AppendVertex(FVector3d(FMath::Cos(Angle) * Length, FMath::Sin(Angle) * Length, 0.0));
		ColorIndexes[i + 1] = ColorOverlay->AppendElement(Color);
	}

	// 頂点の順番が反時計回りになるように、打った点をつなげる
	for (int i = 0; i < CircumferencePointCount; ++i)
	{
		// 最初は中心点、次に i, その次は ((i+1) % (CircumferencePointCount))
		int vertex1=i+1;
		int vertex2=((i+1)%CircumferencePointCount)+1;
		auto TriangleId = Mesh.AppendTriangle(Indexes[0], Indexes[vertex1], Indexes[vertex2]);
		ColorOverlay->SetTriangle(TriangleId, UE::Geometry::FIndex3i(ColorIndexes[0], ColorIndexes[vertex1], ColorIndexes[vertex2]));
	}

	BulletMesh->SetMesh(MoveTemp(Mesh));
	BulletMesh->SetColorOverrideMode(EDynamicMeshComponentColorOverrideMode::VertexColors);
}
