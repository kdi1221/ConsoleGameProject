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

//Vector2Float Room::GetRandomTilePositionInRoom() const
//{
//	const RoomSpace& roomSpace = GetRoomSpace();
//	const RoomSpace::RoomTileIndices& roomTileIndices = roomSpace.GetInnerTileIndices();
//
//	const int randomTileIndex = Util::RandomRange(0, static_cast<int>(roomTileIndices.size() - 1));
//	assert((randomTileIndex >= 0 && randomTileIndex < roomTileIndices.size()) && "Invalid Random Tile Index");
//
//	return static_cast<Vector2Float>(roomTileIndices[randomTileIndex]);
//}

const RoomSpace& Room::GetRoomSpace() const
{
	const RoomSpace* roomSpacePtr = roomSpace.get();
	assert(roomSpacePtr && "Invalid roomSpacePtr");

	return *roomSpacePtr;
}