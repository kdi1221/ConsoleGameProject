#include "AbilityImpFireball.h"

using namespace Craft;

AbilityImpFireball::AbilityImpFireball(int level, eTeamID teamID)
	:super(102, level, 1.f, L"•", Color::Red, 0.025f, 0.04f, teamID, 5.f)
{
}
