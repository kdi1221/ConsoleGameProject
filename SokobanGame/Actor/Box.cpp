#include "Box.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

Box::Box(const Craft::Vector2Int& position)
	:Actor(position)
{
	//필요한 컴포넌트 추가
	//Ground/Target 액터와 겹쳤을때 Box 액터가 위에 그려지도록 정렬 순서 설정.
	AddComponent<SpriteRendererComponent>("B", Craft::Color::Red, 3);
	AddComponent<BoxCollisionComponent>(1);
}