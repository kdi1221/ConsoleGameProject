#pragma once

#include "AbilityProjectile.h"

class AbilityGoblinArrow : public AbilityProjectile
{
	TYPE_DECLARATIONS(AbilityGoblinArrow, AbilityProjectile)

public:
	AbilityGoblinArrow(int level, eTeamID teamID);
	virtual ~AbilityGoblinArrow() = default;
};

