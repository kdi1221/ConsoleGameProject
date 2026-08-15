#include "MapObject.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

MapObject::MapObject(const Craft::Vector2Int& position,
					const std::wstring& image,
					Craft::Color color)
	:super(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::MapObject));
}

bool MapObject::IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor)
{
	/* 기본적으로 맵 오브젝트는 모든 Actor와 겹치기 가능 */
	return false;
}
