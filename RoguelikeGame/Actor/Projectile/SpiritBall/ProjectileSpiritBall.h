#pragma once

#include "Actor/Projectile/Projectile.h"

namespace Craft
{
	class Vector2Int;
	class SpriteRendererComponent;
}

/* 플레이어의 기본 공격 구체 */
class ProjectileSpiritBall : public Projectile
{
	TYPE_DECLARATIONS(ProjectileSpiritBall, Projectile)

public:
	ProjectileSpiritBall(const Craft::Vector2Int& inPosition,
		const float moveSpeed,
		const eTeamID teamID,
		float damageValue);

	virtual ~ProjectileSpiritBall() = default;

private:
	/* 발사체 표시 Sprite Component */
	std::shared_ptr<Craft::SpriteRendererComponent> spriteComponent;
};