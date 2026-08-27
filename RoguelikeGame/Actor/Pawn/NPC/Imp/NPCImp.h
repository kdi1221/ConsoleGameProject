#pragma once

#include "Actor/Pawn/NPC/NPCRangeBase.h"

class NPCImp : public NPCRangeBase
{
	TYPE_DECLARATIONS(NPCImp, NPCRangeBase)

public:
	NPCImp(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCImp() = default;

private:
	virtual ABILITY_ID_TYPE grantRangeAttackAbility() override;
};

