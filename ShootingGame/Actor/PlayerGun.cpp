#include "PlayerGun.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

PlayerGun::PlayerGun(const Vector2& localPosition)
	:Actor(localPosition)
{
	// 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("^", Color::Blue, 6);
}

Vector2 PlayerGun::GetFirePosition() const
{
	// 한 칸 위에서 총알이 발사되도록 위치 설정.
	return GetWorldPosition() + Vector2(0, -1);
}
