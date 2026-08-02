#include "Ground.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

Ground::Ground(const Craft::Vector2& position)
	:Actor(position)
{
	//필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(" ");
}
