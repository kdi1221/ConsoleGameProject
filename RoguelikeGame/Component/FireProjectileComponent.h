#pragma once

#include "Component/Component.h"
#include "Util/Timer.h"
#include "Math/Color.h"
#include "Math/Vector2Float.h"
#include <string>

/* Projectile 발사 컴포넌트 */
class FireProjectileComponent : public Craft::Component
{
	TYPE_DECLARATIONS(FireProjectileComponent, Component)

public:
	FireProjectileComponent(float fireInterval, 
							const std::wstring& inProjectileImage, 
							Craft::Color inProjectileColor,
							float inProjectileMoveDelay);
	virtual ~FireProjectileComponent() = default;

public:
	/* 발사 플래그 설정 */
	void SetEnableFire(bool enable);

	/* projectile 스폰될 offset 지정 */
	void SetProjectileSpawnOffset(const Craft::Vector2Float& offset);

	/* 조준 위치 지정 */
	void SetProjectileAimingPosition(const Craft::Vector2Float& position);

private:
	virtual void Tick(float deltaTime) override;

private:
	void SpawnProjectile();


private:
	/* 발사 동작 여부 */
	bool enableFire = false;

	/* 발사 딜레이 타이머*/
	Timer timerFireInterval;

	/* 스폰할 Projectile Image */
	std::wstring projectileImage = L"";

	/* 스폰할 Projectile Color */
	Craft::Color projectileColor = Craft::Color::White;

	/* 스폰할 Projectile의 이동 딜레이 */
	float projectileMoveDelay = 0.f;

	/* Projectile 스폰 시 Offset(타일 기반)*/
	Craft::Vector2Float projectileSpawnOffset = Craft::Vector2Float::Up;

	/* 조준 위치 */
	Craft::Vector2Float projectileAimingPosition = Craft::Vector2Float::Zero;
};

