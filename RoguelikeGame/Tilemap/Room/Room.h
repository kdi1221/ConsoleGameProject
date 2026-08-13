#pragma once

#include <memory>
#include "Types/Defines.h"
#include "Types/Enums.h"

class RoomSpace;

using namespace RoomDefines;

class Room
{	
public:
	Room(UNIQUE_INDEX_TYPE inUniqueIndex, std::unique_ptr<RoomSpace> inRoomSpace);
	virtual ~Room();

public:
	void SetRoomType(eRoomType newType);
	void SetPlayerVisited(bool inVisited);

public:
	const RoomSpace& GetRoomSpace() const;

public:
	inline eRoomType GetRoomType() const { return roomType; }
	inline UNIQUE_INDEX_TYPE GetRoomUniqueIndex() const { return UniqueIndex; }

private:
	//방의 고유 인덱스
	UNIQUE_INDEX_TYPE UniqueIndex = 0;

	//방의 공간 정보
	std::unique_ptr<RoomSpace> roomSpace = nullptr;

	//방의 종류
	eRoomType roomType = eRoomType::None;
};

