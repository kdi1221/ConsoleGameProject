#pragma once

#include "AbilityProjectile.h"

class AbilitySpiritBall : public AbilityProjectile
{
	TYPE_DECLARATIONS(AbilitySpiritBall, AbilityProjectile)

public:
	AbilitySpiritBall(int level, eTeamID teamID);
	virtual ~AbilitySpiritBall() = default;

protected:
	virtual void PlayShootSound() override;
};

