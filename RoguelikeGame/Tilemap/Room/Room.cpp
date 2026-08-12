#include "Room.h"
#include "Tilemap/Tilemap.h"
#include "TileMap/BSP/RoomSpace/RoomSpace.h"
#include "Math/Vector2Int.h"
#include "Util/Util.h"
#include "Level/Level.h"
#include <cassert>

using namespace Craft;

Room::Room(UNIQUE_INDEX_TYPE inUniqueIndex, std::unique_ptr<RoomSpace> inRoomSpace)
	:UniqueIndex(inUniqueIndex)
	,roomSpace(std::move(inRoomSpace))
{

}

Room::~Room()
{

}

void Room::SetRoomType(eRoomType newType)
{
	roomType = newType;
}

void Room::SetPlayerVisited(bool inVisited)
{
	isPlayerVisited = inVisited;
}

const RoomSpace& Room::GetRoomSpace() const
{
	const RoomSpace* roomSpacePtr = roomSpace.get();
	assert(roomSpacePtr && "Invalid roomSpacePtr");

	return *roomSpacePtr;
}