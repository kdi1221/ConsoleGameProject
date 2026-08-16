#pragma once

#include "Actor/Pawn/NPC/NPCRangeBase.h"

class NPCGoblinArcher : public NPCRangeBase
{
	TYPE_DECLARATIONS(NPCGoblinArcher, NPCRangeBase)

public:
	NPCGoblinArcher(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCGoblinArcher() = default;

private:
	virtual AbilityObject::ABILITY_ID_TYPE grantRangeAttackAbility() override;
};

