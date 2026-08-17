#include "NPCImp.h"
#include "Ability/Shooter/Projectile/AbilityImpFireball.h"
#include <Util/Util.h>
#include <cassert>

using namespace Craft;

NPCImp::NPCImp(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"I", Color::Red, 35.f, roomIndex, 0.08f, 0.5f)
{
	SetFireRange(10.f);
}

AbilityObject::ABILITY_ID_TYPE NPCImp::grantRangeAttackAbility()
{
	std::shared_ptr<AbilitySystemComponent> abilitySystemComponent = GetAbilitySystemComponent();
	assert(abilitySystemComponent && "Invalid abilitySystemComponent");

	return abilitySystemComponent->AddNewAbility<AbilityImpFireball>(Util::RandomRange(1, 2), GetTeamID());
}
