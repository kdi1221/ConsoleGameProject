#include "ProjectileIceBolt.h"
#include "Component/SpriteRendererComponent.h"
#include <Defines/Enums.h>

using namespace Craft;

ProjectileIceBolt::ProjectileIceBolt(const Vector2Int& inPosition, const float moveSpeed, const eTeamID teamID, float damageValue)
	:super(inPosition, moveSpeed, teamID, damageValue)
{
	SetImage(L"◥");
	SetImageColor(Color::LightBlue);

	SetLifeSpan(1.f);
}

void ProjectileIceBolt::SetIceBoltImage(const eDirection direction)
{
	std::wstring boltImage;
	switch (direction)
	{
	case eDirection::Left: boltImage = L"◂"; break;
	case eDirection::Top: boltImage = L"▴"; break;
	case eDirection::Right: boltImage = L"▸"; break;
	case eDirection::Bottom: boltImage = L"▾"; break;
	case eDirection::LeftTop: boltImage = L"◤"; break;
	case eDirection::RightTop: boltImage = L"◥"; break;
	case eDirection::LeftBottom: boltImage = L"◣"; break;
	case eDirection::RightBottom: boltImage = L"◢"; break;
	}

	SetImage(boltImage);
}
	