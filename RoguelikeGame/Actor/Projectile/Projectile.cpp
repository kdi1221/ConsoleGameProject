#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "Component/PathMoveComponent.h"
#include "Types/Enums.h"
#include "Actor/MapObject/RoomDoor.h"
#include "Actor/Pawn/Pawn.h"
#include <cassert>
#include <StaticLibrary/StaticFunctionLibrary.h>

using namespace Craft;

Projectile::Projectile(const Craft::Vector2Int& inPosition,
						const std::wstring& inImage,
						Craft::Color inColor,
						const Craft::Vector2Int& inDestination,
						float moveDelay,
						eTeamID teamID,
						int damage)
	:super(inPosition)
	,destinationPos(inDestination)
	,instigatorTeamID(teamID)
	,damageValue(damage)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(inImage, inColor, static_cast<int>(eRenderSortingOrder::Projectile));
	AddComponent<BoxCollisionComponent>(1);
	pathMoveComponent = AddComponent<PathMoveComponent>(moveDelay, false);
	pathMoveComponent->SetMoveFinishCallback(std::bind(&Projectile::OnCallbackMoveFinish, this));
}

void Projectile::BeginPlay()
{
	super::BeginPlay();

	/* 브레젠험 알고리즘, 시작 지점과 끝 지점을 잇는 선 경로를 가져옴 */
	const Vector2Int startPostion = GetWorldPosition();
	std::vector<Vector2Int> movePaths;
	StaticFunctionLibrary::GetBresenhamPath(startPostion, destinationPos, movePaths);

	/* 지정된 목적지를 향해 경로를 지정하고 이동 시작 */
	assert(pathMoveComponent && "pathMoveComponent invalid..");
	pathMoveComponent->StartMove(std::move(movePaths));
}

void Projectile::OnTileOverlap(const eTileCategory tileCategory)
{
	super::OnTileOverlap(tileCategory);

	switch (tileCategory)
	{
	case eTileCategory::Wall:
		{
			/* 현재 겹친 타일이 벽타일인 경우 => 제거 */
			Destroy();
		}
		break;
	}
}

bool Projectile::IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor)
{
	/* 상관없이 모두 겹치기 가능 */
	return false;
}

void Projectile::OnCollision(const std::shared_ptr<Actor>& other)
{
	if (other->IsTypeOf<RoomDoor>())
	{
		/* 방을 막고 있는 문 오브젝트와 충돌하면 Destroy */
		Destroy();
		return;
	}

	if (other->IsTypeOf<Pawn>())
	{
		/* 다른 소속 팀의 Pawn과 충돌했으면 데미지를 입히고 Destory */
		std::shared_ptr<Pawn> HitPawn = Cast<Pawn>(other);
		if (HitPawn && HitPawn->GetTeamID() != instigatorTeamID)
		{
			HitPawn->TakeDamage(damageValue);
			Destroy();
		}
	}
}

void Projectile::OnCallbackMoveFinish()
{
	/* 목적지에 도착했으면 Destory 처리 */
	Destroy();
}
