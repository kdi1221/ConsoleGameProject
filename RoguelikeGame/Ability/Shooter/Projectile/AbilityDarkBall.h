#pragma once

#include "AbilityProjectile.h"

class AbilityDarkBall : public AbilityProjectile
{
	TYPE_DECLARATIONS(AbilityDarkBall, AbilityProjectile)

public:
	AbilityDarkBall(int level, eTeamID teamID);
	virtual ~AbilityDarkBall() = default;
};

