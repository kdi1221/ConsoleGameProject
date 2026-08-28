#include "ProjectileIceBolt.h"

using namespace Craft;

ProjectileIceBolt::ProjectileIceBolt(const Vector2Int& inPosition, const float moveSpeed, const eTeamID teamID, float damageValue)
	:super(inPosition, L"◥", Color::LightBlue, moveSpeed, teamID, damageValue)
{
	SetLifeSpan(1.f);
}
