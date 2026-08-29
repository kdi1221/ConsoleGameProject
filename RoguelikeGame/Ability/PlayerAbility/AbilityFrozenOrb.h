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
};

