#pragma once

#include "Ability/AbilityPeriodicDelay.h"
#include <Util/Timer.h>

/* 발사체를 발사하는 Ability Base */
class AbilityShooter : public AbilityPeriodicDelay
{
	TYPE_DECLARATIONS(AbilityShooter, AbilityPeriodicDelay)

public:
	AbilityShooter(ABILITY_ID_TYPE id, int level, float fireDelay);
	virtual ~AbilityShooter() = default;
};

