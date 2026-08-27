#pragma once

#include "Ability/AbilityObject.h"
#include <Math/Color.h>
#include <string>

/* Spirit Ball 발사 Ability */
class AbilityShotSpiritBall : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityShotSpiritBall, AbilityObject)

public:
	AbilityShotSpiritBall(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityShotSpiritBall();

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;

	/* Ability 종료 */
	virtual void EndAbility(bool bCancelAbility) override;

	/* Ability 취소 */
	virtual void CancelAbility() override;

private:
	/* 생성할 Projectile의 Image */
	std::wstring spawnProjectileImage = L"•";

	/* 생성할 Projectile의 Color */
	Craft::Color spawnProjectileColor = Craft::Color::LightGreen;
};

