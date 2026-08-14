#pragma once

#include "AbilityShooter.h"
#include "Types/Enums.h"
#include <string>
#include <Math/Color.h>
#include <Math/Vector2Int.h>

/* 탄환 발사하는 Ability */
class AbilityProjectile : public AbilityShooter
{
	TYPE_DECLARATIONS(AbilityProjectile, AbilityShooter)

public:
	AbilityProjectile(float fireDelay,
						const std::wstring image,
						const Craft::Color color,
						float moveDelayMin,
						float moveDelayMax,
						eTeamID teamID,
						float damageValue);

	virtual ~AbilityProjectile() = default;

public:
	void SetProjectileSpawnOffset(const Craft::Vector2Int& spawnOffset);
	void SetAimingPostion(const Craft::Vector2Int& position);

private:
	/* 딜레이 타이머 동작 */
	virtual void OnIntervalTrigger() override;

private:
	/* 스폰할 Projectile Image */
	std::wstring projectileImage = L"";

	/* 스폰할 Projectile Color */
	Craft::Color projectileColor = Craft::Color::White;

	/* 스폰할 Projectile의 이동 딜레이(최소) */
	float projectileMoveDelayMin = 0.f;

	/* 스폰할 Projectile의 이동 딜레이(최대) */
	float projectileMoveDelayMax = 0.f;

	/* 탄환을 발사하는 행위자의 Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 생성할 탄환의 데미지*/
	float projectileDamage = 0;

	/* Projectile 스폰 시 Offset(타일 기반)*/
	Craft::Vector2Int projectileSpawnOffset = Craft::Vector2Int::Up;

	/* 조준 위치 */
	Craft::Vector2Int aimingPosition = Craft::Vector2Int::Zero;
};

