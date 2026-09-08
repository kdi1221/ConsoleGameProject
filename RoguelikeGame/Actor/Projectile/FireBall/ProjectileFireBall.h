#pragma once

#include "Actor/Projectile/Projectile.h"
#include "Types/Defines.h"
#include <unordered_set>

/* 파이어볼 구체 */
class ProjectileFireBall : public Projectile
{
	TYPE_DECLARATIONS(ProjectileFireBall, Projectile)

public:
	ProjectileFireBall(const Craft::Vector2Int& inPosition,
						const float moveSpeed,
						const eTeamID teamID,
						float damageValue,
						float explosionRange,
						float explosionDamage);

	virtual ~ProjectileFireBall() = default;

private:
	/* 벽과 충돌했을때 호출(true면 destroy, false면 유지) */
	virtual bool OnBlockWall(const Craft::Vector2Int& blockPosition, const Craft::Vector2Int& lastNonBlockPosition) override;

	/* 다른 Actor와 충돌했을때 호출(true면 destroy, false면 유지) */
	virtual bool OnBlockActor(std::shared_ptr<ActorOnTile> blockingActor, const Craft::Vector2Int& blockPosition, const Craft::Vector2Int& lastNonBlockPosition) override;

private:
	/* 범위 데미지 적용 */
	void ApplyExplosionDamage(const Craft::Vector2Int& centerPosition);

	/* 충돌한 위치에 폭발 이펙트 생성 */
	void SpawnExplosionFX(const Craft::Vector2Int& spawnPosition);

private:
	/* 폭발 범위 */
	float explosionRange = 0.f;

	/* 폭발 데미지 */
	float explosionDamage = 0.f;

	/* 데미지를 준 Pawn 정보 저장(중복 데미지 처리 방지) */
	std::unordered_set<PawnUniqueIDType> setDamagedPawns;
};

