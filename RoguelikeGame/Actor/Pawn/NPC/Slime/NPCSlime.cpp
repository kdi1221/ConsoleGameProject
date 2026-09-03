#include "NPCSlime.h"
#include "Ability/NPCAbility/Melee/AbilityMelee.h"
#include <Math/Color.h>
#include <cassert>

using namespace Craft;

NPCSlime::NPCSlime(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"s", Color::Purple, 25.f, 5.f, 3.f, 2.f, 0.5f, roomIndex)
{

}