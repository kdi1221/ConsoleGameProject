#include "Wall.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

Wall::Wall(const Craft::Vector2Float& position)
	: Actor(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("#", Color::White, 2);
	AddComponent<BoxCollisionComponent>(1);
}
