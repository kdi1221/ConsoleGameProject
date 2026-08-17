#pragma once

#include "Actor/Pawn/NPC/NPCMeleeBase.h"

//몬스터 - 오크
class NPCOrc : public NPCMeleeBase
{
	TYPE_DECLARATIONS(NPCOrc, NPCMeleeBase)

public:
	NPCOrc(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCOrc() = default;
};

