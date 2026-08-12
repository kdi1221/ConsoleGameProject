#pragma once

#include "Math/Vector2Int.h"
#include "Types/Defines.h"
#include "Types/Enums.h"

//타일맵 개별 타일 클래스
class Tile
{
public:
	Tile(eTileCategory InCategory, const Craft::Vector2Int& InPosition, size_t InTileIndex);
	virtual ~Tile();

public:
	//Getter
	inline eTileCategory GetTileCategory() const { return category; }
	inline const Craft::Vector2Int& GetTilePosition() const { return position; }
	inline size_t GetTileIndex() const { return index; }
	inline RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndex() const { return roomIndex; }

	//Setter
	inline void SetTileCategory(eTileCategory InCategory) { category = InCategory; }
	inline void SetTileRoomIndex(RoomDefines::UNIQUE_INDEX_TYPE inIndex) { roomIndex = inIndex; }

private:
	//타일 종류
	eTileCategory category = eTileCategory::None;

	//타일 위치(2차원 좌표 x, y)
	Craft::Vector2Int position = Craft::Vector2Int::Zero;

	//타일 인덱스(배열 상 고유 인덱스)
	size_t index = -1;

	//타일이 속해있는 방의 인덱스
	RoomDefines::UNIQUE_INDEX_TYPE roomIndex = RoomDefines::ROOM_INDEX_INVALID;
};

