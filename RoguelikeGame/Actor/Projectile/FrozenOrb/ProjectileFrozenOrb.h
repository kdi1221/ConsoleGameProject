#pragma once

#include "Actor/Projectile/Projectile.h"
#include <Util/Timer.h>

namespace Craft
{
	class Vector2Int;
	class SpriteRendererComponent;
}

/* 프로즌오브 중심 구체 */
class ProjectileFrozenOrb : public Projectile
{
	TYPE_DECLARATIONS(ProjectileFrozenOrb, Projectile)

public:
	ProjectileFrozenOrb(const Craft::Vector2Int& inPosition,
						const float moveSpeed,
						const eTeamID teamID,
						float damageValue,
						float spawnIceboltDelay);

	virtual ~ProjectileFrozenOrb() = default;

public:
	virtual void Tick(float deltaTime) override;

public:
	/* 매 딜레이 얼음살 생성 수 지정 */
	void SetSpawnIceBoltNum(int IceBoltNum);

	/* 생성되는 얼음살의 이동속도 지정 */
	void SetSpawnIceBoltMoveSpeed(float moveSpeed);

	/* 생성되는 얼음살의 데미지 지정 */
	void SetSpawnIceBoltDamage(float damageAmount);

private:
	/* 얼음살 생성 */
	void SpawnIceBolts();

private:
	/* 오브 구체의 시간 누적 */
	float accumulationTime = 0.f;

	/* 얼음살 발사 딜레이 타이머 */
	Timer timerSpawnIceBoltDelay;

	/* 회전 속도 */
	float rotateSpeed = 180.f;

	/* 매 딜레이 얼음살 생성 수 */
	int spawnIceBoltNum = 4;

	/* 얼음살 이동 속도*/
	float moveSpeedIceBolt = 30.f;

	/* 얼음살 Damage */
	float damageIceBolt = 5.f;

private:
	/* 구체 표시 Sprite Component*/
	std::shared_ptr<Craft::SpriteRendererComponent> spriteComponent;

};

