#include "PlayerBullet.h"

using namespace Craft;

PlayerBullet::PlayerBullet(const Craft::Vector2& position)
	:super("@", position, Color::Blue)
	,yPosition(static_cast<float>(position.y))
{

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

	//위치 설정
	position.y = static_cast<int>(yPosition);
}
