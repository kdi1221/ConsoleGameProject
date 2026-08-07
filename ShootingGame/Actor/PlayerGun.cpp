#include "PlayerGun.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector2Float.h"

using namespace Craft;

PlayerGun::PlayerGun(const Vector2Float& localPosition)
	:Actor(localPosition)
{
	// 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>("^", Color::Blue, 6);
}

Vector2Float PlayerGun::GetFirePosition() const
{
	// 한 칸 위에서 총알이 발사되도록 위치 설정.
	return GetWorldPosition() + Vector2Float(0.f, -1.f);
}
