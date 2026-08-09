#include "MapObject.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

MapObject::MapObject(const Craft::Vector2Float& position, 
					const std::string& image, 
					Craft::Color color)
	:super(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::MapObject));
	AddComponent<BoxCollisionComponent>(1);
}