#pragma once

#include <memory>
#include "Types/Defines.h"

class Tilemap;
class RoomSpace;

using namespace RoomDefines;

class Room
{
public:
	Room(Tilemap& inOwnerTilemap, UNIQUE_INDEX_TYPE inUniqueIndex, std::unique_ptr<RoomSpace> inRoomSpace);
	virtual ~Room();

public:
	const RoomSpace& GetRoomSpace() const;

private:
	// 이 방을 소유하는 타일맵
	Tilemap& ownerTilemap;

	//방의 고유 인덱스
	UNIQUE_INDEX_TYPE UniqueIndex = 0;

	//방의 공간 정보
	std::unique_ptr<RoomSpace> roomSpace = nullptr;
};

