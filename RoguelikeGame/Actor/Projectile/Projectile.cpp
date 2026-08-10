#include "Projectile.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include "Component/MovementComponent.h"
#include "Types/Enums.h"


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
	movementComponent = AddComponent<MovementComponent>(moveDelay);
}

void Projectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//TODO : 브레젠험 알고리즘을 통해 얻은 경로대로 이동 필요..
}