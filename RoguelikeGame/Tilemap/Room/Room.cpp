#include "Room.h"
#include "Tilemap/Tilemap.h"
#include "TileMap/BSP/RoomSpace/RoomSpace.h"
#include "Math/Vector2.h"
#include <cassert>

using namespace Craft;

Room::Room(Tilemap& inOwnerTilemap, UNIQUE_INDEX_TYPE inUniqueIndex, std::unique_ptr<RoomSpace> inRoomSpace)
	:ownerTilemap(inOwnerTilemap)
	,UniqueIndex(inUniqueIndex)
	,roomSpace(std::move(inRoomSpace))
{

}

Room::~Room()
{

}

const RoomSpace& Room::GetRoomSpace() const
{
	const RoomSpace* roomSpacePtr = roomSpace.get();
	assert(roomSpacePtr && "Invalid roomSpacePtr");

	return *roomSpacePtr;
}
