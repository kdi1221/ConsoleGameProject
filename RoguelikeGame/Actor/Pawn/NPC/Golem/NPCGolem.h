#pragma once

#include "Actor/Pawn/NPC/NPCMeleeBase.h"

// 몬스터 - 골렘
class NPCGolem : public NPCMeleeBase
{
	TYPE_DECLARATIONS(NPCGolem, NPCMeleeBase)

public:
	NPCGolem(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex);
	virtual ~NPCGolem() = default;
};

