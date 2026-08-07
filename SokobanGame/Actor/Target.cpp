#include "Target.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

Target::Target(const Craft::Vector2Float& position)
	:Actor(position)
{
	//필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("T", Color::Blue, 1);
}
