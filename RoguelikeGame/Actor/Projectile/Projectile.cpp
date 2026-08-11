#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Types/Enums.h"
#include <cassert>
#include <StaticLibrary/StaticFunctionLibrary.h>

using namespace Craft;

Projectile::Projectile(const Craft::Vector2Float& inPosition,
						const std::wstring& inImage,
						Craft::Color inColor,
						const Craft::Vector2Float& inDestination,
						float moveDelay)
	:super(inPosition)
	,destinationPos(inDestination)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(inImage, inColor, static_cast<int>(eRenderSortingOrder::Projectile));
	AddComponent<BoxCollisionComponent>(1);
	pathMoveComponent = AddComponent<PathMoveComponent>(moveDelay, std::bind(&Projectile::OnCallbackMoveFinish, this));
}

void Projectile::BeginPlay()
{
	super::BeginPlay();

	assert(pathMoveComponent && "pathMoveComponent invalid..");

	//TODO : 시작위치에서 충돌감지되었을때 Destory

	pathMoveComponent->StartMoveToPosition(static_cast<Vector2Int>(destinationPos));
}

void Projectile::OnCallbackMoveFinish()
{
	/* 목적지에 도착했으면 Destory 처리 */
	Destroy();
}
