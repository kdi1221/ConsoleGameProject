#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Types/Enums.h"

using namespace Craft;

Projectile::Projectile(const Vector2Int& inPosition,
						const std::wstring& inImage,
						Craft::Color inColor)
	:super(inPosition)
{
	spriteComponent = AddComponent<SpriteRendererComponent>(inImage, inColor, static_cast<int>(eRenderSortingOrder::Projectile));
}
