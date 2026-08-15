#include "FieldItem.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

FieldItem::FieldItem(const Vector2Int& position, const std::wstring& image, Color color)
	:super(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::FieldItem));
}

bool FieldItem::IsBlockActorOnTile(std::shared_ptr<ActorOnTile> otherActor)
{
	/* 기본적으로 필드 아이템은 모든 Actor와 겹치기 가능 */
	return false;
}
