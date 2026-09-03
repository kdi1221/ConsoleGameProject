#include "ProjectileSpiritBall.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

ProjectileSpiritBall::ProjectileSpiritBall(const Vector2Int& inPosition, const float moveSpeed, const eTeamID teamID, float damageValue)
	:super(inPosition, moveSpeed, teamID, damageValue)
{
	SetImage(L"•");
	SetImageColor(Color::LightGreen);

	SetLifeSpan(1.f);
}