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

/* 게임 내 표시되는 모든 발사체들의 베이스 클래스 */
class Projectile : public Craft::Actor
{
	TYPE_DECLARATIONS(Projectile, Actor)

public:
	Projectile(const Craft::Vector2Int& inPosition,
				const std::wstring& inImage,
				Craft::Color inColor,
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
	/* 발사체 표시 Sprite Component */
	std::shared_ptr<Craft::SpriteRendererComponent> spriteComponent;

	/* 발사체 이동 Component */
	std::shared_ptr<Craft::ProjectileMoveComponent> projectileMoveComponent;
};

