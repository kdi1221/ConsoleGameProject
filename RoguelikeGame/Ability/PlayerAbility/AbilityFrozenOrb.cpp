#include "AbilityFrozenOrb.h"
#include <Windows.h>

AbilityFrozenOrb::AbilityFrozenOrb(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(10.f);
	SetManaCost(15.f);
}

void AbilityFrozenOrb::ActivateAbility()
{
	OutputDebugStringA("TODO : ActivateAbility FrozeOrb\n");

	EndAbility(false);
}
