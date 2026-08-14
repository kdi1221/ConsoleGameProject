#pragma once

#include "Actor/Pawn/NPC/NPCMeleeBase.h"

//몬스터 - 슬라임
class NPCSlime : public NPCMeleeBase
{
	TYPE_DECLARATIONS(NPCSlime, NPCMeleeBase)

public:
	NPCSlime(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCSlime() = default; 
};

