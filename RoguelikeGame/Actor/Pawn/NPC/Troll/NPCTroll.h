#pragma once

#include "Actor/Pawn/NPC/NPCMeleeBase.h"

//몬스터 - 트롤
class NPCTroll : public NPCMeleeBase
{
	TYPE_DECLARATIONS(NPCTroll, NPCMeleeBase)

public:
	NPCTroll(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCTroll() = default;
};

