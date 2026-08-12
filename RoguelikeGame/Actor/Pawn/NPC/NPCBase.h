#pragma once

#include "Actor/Pawn/Pawn.h"
#include "Types/Defines.h"

/* 모든 NPC(몬스터)의 상위 클래스 */
class NPCBase : public Pawn
{
	TYPE_DECLARATIONS(NPCBase, Pawn)

public:
	NPCBase(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		int CollisionWidth,
		int initialHealth);

	~NPCBase() = default;
};

