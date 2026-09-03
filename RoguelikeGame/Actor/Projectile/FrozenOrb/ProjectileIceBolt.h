#pragma once

#include "Actor/Projectile/Projectile.h"

namespace Craft
{
	class Vector2Int;
	class SpriteRendererComponent;

	enum class eDirection : unsigned int;
}

/* 프로즌 오브 구체 중심으로 퍼져나가는 얼음살 */
class ProjectileIceBolt : public Projectile
{
	TYPE_DECLARATIONS(ProjectileIceBolt, Projectile)

public:
	ProjectileIceBolt(const Craft::Vector2Int& inPosition,
					const float moveSpeed,
					const eTeamID teamID,
					float damageValue);

	virtual ~ProjectileIceBolt() = default;

public:
	void SetIceBoltImage(const Craft::eDirection direction);
};

