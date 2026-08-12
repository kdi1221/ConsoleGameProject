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
		int initialHealth,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex);

	~NPCBase() = default;

public:
	inline RoomDefines::UNIQUE_INDEX_TYPE GetSpawnedRoomIndex() const { return spawnedRoomIndex; }

private:
	//생성된 방의 인덱스
	RoomDefines::UNIQUE_INDEX_TYPE spawnedRoomIndex = RoomDefines::ROOM_INDEX_INVALID;
};

