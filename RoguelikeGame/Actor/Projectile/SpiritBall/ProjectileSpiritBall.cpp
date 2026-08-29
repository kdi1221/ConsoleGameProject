#include "ProjectileSpiritBall.h"
#include "Component/SpriteRendererComponent.h"

using namespace Craft;

ProjectileSpiritBall::ProjectileSpiritBall(const Vector2Int& inPosition, const float moveSpeed, const eTeamID teamID, float damageValue)
	:super(inPosition, moveSpeed, teamID, damageValue)
{
	spriteComponent = AddComponent<SpriteRendererComponent>(L"•", Color::LightGreen, static_cast<int>(eRenderSortingOrder::Projectile));

	SetLifeSpan(1.f);
}