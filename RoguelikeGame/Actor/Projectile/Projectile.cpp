#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "Types/Enums.h"

namespace Craft
{
	Projectile::Projectile(const Craft::Vector2Float& inPosition,
							const std::wstring& inImage,
							Craft::Color inColor,
							float inRange,
							const Craft::Vector2Float& inDirection)
		:super(inPosition)
		,range(inRange)
		,direction(inDirection)
	{
		// 필요한 컴포넌트 추가.
		AddComponent<SpriteRendererComponent>(inImage, inColor, static_cast<int>(eRenderSortingOrder::Projectile));
		AddComponent<BoxCollisionComponent>(1);
	}

	void Projectile::Tick(float deltaTime)
	{
		super::Tick(deltaTime);


	}
}