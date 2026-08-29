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
	virtual ~AbilityShotSpiritBall() = default;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;
};

