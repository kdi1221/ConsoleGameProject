#include "PlayerBullet.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"

using namespace Craft;

PlayerBullet::PlayerBullet(const Craft::Vector2Int& position)
	:super(position)
	,yPosition(static_cast<float>(position.y))
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("@", Color::Blue, 4);
	AddComponent<BoxCollisionComponent>(1);
}

void PlayerBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//위치 이동 처리
	yPosition -= moveSpeed * deltaTime;

	// 좌표 검사.
	if (yPosition < 0.f)
	{
		// 삭제 처리
		Destroy();
		return;
	}

	//position.y = static_cast<int>(yPosition);
	//위치 설정 - 컴포넌트 기반으로 동작하도록 처리
	Vector2Int newPosition = GetPosition();
	newPosition.y = static_cast<int>(yPosition);
	SetPosition(newPosition);
}
