#include "EnemyBullet.h"
#include "Engine/Engine.h"
#include "Engine/Config/ConfigBase.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

EnemyBullet::EnemyBullet(const Craft::Vector2Int& position, float moveSpeed)
	:Actor(position)
	,moveSpeed(moveSpeed)
	,yPosition(static_cast<float>(position.y))
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("#", Color::Red, 4);
	AddComponent<BoxCollisionComponent>(1);
}

void EnemyBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// y위치 업데이트.
	yPosition += moveSpeed * deltaTime;

	// 좌표 검사.
	if (yPosition >= Engine::Get().GetConfig<ConfigBase>().GetDisplayHeight() - 1)
	{
		Destroy();
		return;
	}

	//위치 설정.
	//SetPosition(Vector2(GetPosition().x, static_cast<int>(yPosition)));
	Vector2Int newPosition = GetPosition();
	newPosition.y = static_cast<int>(yPosition);
	SetPosition(newPosition);
}
