#pragma once

#include "Ability/AbilityObject.h"

//플레이어 스킬 - 프로즌 오브
class AbilityFrozenOrb : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityFrozenOrb, AbilityObject)

public:
	AbilityFrozenOrb(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityFrozenOrb() = default;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;

private:
	/* 생성할 Projectile의 Image */
	//std::wstring spawnProjectileImage = L"•";

	/* 생성할 Projectile의 Color */
	//Craft::Color spawnProjectileColor = Craft::Color::LightGreen;
};

