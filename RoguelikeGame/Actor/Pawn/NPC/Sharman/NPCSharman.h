#pragma once

#include "Actor/Pawn/NPC/NPCRangeBase.h"

class NPCSharman : public NPCRangeBase
{
	TYPE_DECLARATIONS(NPCSharman, NPCRangeBase)

public:
	NPCSharman(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCSharman() = default;

private:
	virtual ABILITY_ID_TYPE grantRangeAttackAbility() override;
};

