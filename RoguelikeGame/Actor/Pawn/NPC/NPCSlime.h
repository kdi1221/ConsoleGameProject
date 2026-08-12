#pragma once

#include "NPCBase.h"

//몬스터 - 슬라임
class NPCSlime : public NPCBase
{
	TYPE_DECLARATIONS(NPCSlime, NPCBase)

public:
	NPCSlime(const Craft::Vector2Int& position);
	virtual ~NPCSlime() = default; 
};

