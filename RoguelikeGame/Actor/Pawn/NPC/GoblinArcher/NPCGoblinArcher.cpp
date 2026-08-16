#include "NPCGoblinArcher.h"
#include "Ability/Shooter/Projectile/AbilityGoblinArrow.h"
#include <cassert>

using namespace Craft;

NPCGoblinArcher::NPCGoblinArcher(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"g", Color::Green, 15.f, roomIndex, 0.15f, 0.5f)
{
	SetFireRange(8.f);
}

AbilityObject::ABILITY_ID_TYPE NPCGoblinArcher::grantRangeAttackAbility()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	return abilitySystemComponent->AddNewAbility<AbilityGoblinArrow>(1, GetTeamID());
}
