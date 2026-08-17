#pragma once

#include "AbilityProjectile.h"

class AbilityImpFireball : public AbilityProjectile
{
	TYPE_DECLARATIONS(AbilityImpFireball, AbilityProjectile)

public:
	AbilityImpFireball(int level, eTeamID teamID);
	virtual ~AbilityImpFireball() = default;
};

