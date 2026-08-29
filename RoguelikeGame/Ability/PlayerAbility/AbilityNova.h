#pragma once

#include "Ability/AbilityObject.h"

//플레이어 스킬 노바
class AbilityNova : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityNova, AbilityObject)

public:
	AbilityNova(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityNova() = default;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;
};

