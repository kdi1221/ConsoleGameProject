#pragma once

#include "Types/Enums.h"
#include <Util/Timer.h>
#include <Math/Vector2Int.h>
#include <Actor/Actor.h>

namespace Craft
{
	class SpriteRendererComponent;
	class ProjectileMoveComponent;
}

class ActorOnTile;

/* 게임 내 표시되는 모든 발사체들의 베이스 클래스 */
class Projectile : public Craft::Actor
{
	TYPE_DECLARATIONS(Projectile, Actor)

public:
	Projectile(const Craft::Vector2Int& inPosition,
				const float moveSpeed,
				const eTeamID teamID,
				float damageValue);
	virtual ~Projectile() = default;

public:
	virtual void PreTick(float deltaTime) override;
	virtual void Tick(float deltaTime) override;
	virtual void PostTick(float deltaTime) override;

public:
	/* Projectile의 이동 방향 지정 */
	void SetMoveDirection(const Craft::Vector2Float& moveDirection);

	/* Projectile의 수명 설정 */
	void SetLifeSpan(float lifeTime);

public:
	/* 발사체에 지정된 TeamID반환(피아식별) */
	inline eTeamID GetInstigatorTeamID() const { return instigatorTeamID; }

	/* 발사체의 데미지 */
	inline float GetDamageValue() const { return damageValue; }

protected:
	/* 벽과 충돌했을때 호출(true면 destroy, false면 유지) */
	virtual bool OnBlockWall();

	/* 다른 Actor와 충돌했을때 호출(true면 destroy, false면 유지) */
	virtual bool OnBlockActor(std::shared_ptr<ActorOnTile> blockingActor);

protected:
	/* 이동 방향 반환 */
	const Craft::Vector2Float& GetMoveDirection() const;

private:
	/* 이동 후 충돌 여부 확인 */
	void PostMoveCheckBlockCollision();

private:
	/* 이동 전 위치 */
	Craft::Vector2Int prevPosition = Craft::Vector2Int::Zero;

	/* Instigator Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* Damage Value*/
	float damageValue = 0.f;

	/* Projectile의 수명 설정 여부 */
	bool isLifeSpan = false;

	/* Projectile의 수명 타이머 */
	Timer timerLifeSpan;

private:
	/* 발사체 이동 Component */
	std::shared_ptr<Craft::ProjectileMoveComponent> projectileMoveComponent;
};

