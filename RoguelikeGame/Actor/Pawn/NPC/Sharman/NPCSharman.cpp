#include "NPCSharman.h"
#include "Ability/Shooter/Projectile/AbilityDarkBall.h"
#include <Util/Util.h>
#include <cassert>

using namespace Craft;

NPCSharman::NPCSharman(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"S", Color::Yellow, 50.f, roomIndex, 0.12f, 0.5f)
{
	SetFireRange(12.f);
}

ABILITY_ID_TYPE NPCSharman::grantRangeAttackAbility()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	return abilitySystemComponent->AddNewAbility<AbilityDarkBall>(Util::RandomRange(2, 3), GetTeamID());
}
