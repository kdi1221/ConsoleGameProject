#pragma once

#include <memory>
#include "Types/Defines.h"
#include "Types/Enums.h"
#include "Math/Vector2Float.h"

class RoomSpace;

using namespace RoomDefines;

class Room
{	
public:
	Room(UNIQUE_INDEX_TYPE inUniqueIndex, std::unique_ptr<RoomSpace> inRoomSpace);
	virtual ~Room();

public:
	void SetRoomType(eRoomType newType);

public:
	//Craft::Vector2Float GetRandomTilePositionInRoom() const;
	const RoomSpace& GetRoomSpace() const;

public:
	inline eRoomType GetRoomType() const { return roomType; }

private:
	//방의 고유 인덱스
	UNIQUE_INDEX_TYPE UniqueIndex = 0;

	//방의 공간 정보
	std::unique_ptr<RoomSpace> roomSpace = nullptr;

	//방의 종류
	eRoomType roomType = eRoomType::None;
};

