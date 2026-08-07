#include "Tilemap.h"
#include "Render/Renderer.h"
#include "Tile.h"
#include "BSP/BSPNode.h"
#include "BSP/RoomSpace/RoomSpace.h"
#include "Room/Room.h"
#include "Util/Util.h"
#include <cassert>

using namespace Craft;

Tilemap::Tilemap()
{
	
}

Tilemap::~Tilemap()
{

}

void Tilemap::InitializeTilemap(const Vector2Int& inMapSize)
{
	mapSize = inMapSize;

	//생성할 타일 갯수
	const size_t GenerateMapTileNum = mapSize.x * mapSize.y;

	//벡터 공간 미리 확보
	tileList.reserve(GenerateMapTileNum);

	//맵 내의 타일 초기화 및 벽으로 채우기
	for (size_t TileIndex = 0; TileIndex < GenerateMapTileNum; ++TileIndex)
	{
		const int PosX = static_cast<int>(TileIndex % mapSize.x);
		const int PosY = static_cast<int>(TileIndex / mapSize.x);

		tileList.emplace_back(std::make_unique<Tile>(eTileCategory::Wall, Vector2Int(PosX, PosY), TileIndex));
	}

	//BSP알고리즘을 통한 공간 분할 및 방 생성, 경로 정보 생성
	std::unique_ptr<BSPNode> BSPRoot = std::make_unique<BSPNode>(0, 0, mapSize.x, mapSize.y);
	assert(BSPRoot && "BSPRoot Alloc Failed..");
	BSPRoot->Divide();

	//분할된 공간을 연결
	BSPRoot->ConnectRooms();

	//방, 경로 정보들을 바탕으로 Tile 구성
	BSPRoot->ExtractNodeContents(
		[this](const std::vector<Craft::Vector2Int>& pathTileIndices)
		{
			for (const Vector2Int& pathTileIndex : pathTileIndices)
			{
				//경로상의 타일 뚫기
				SetTileCategory(pathTileIndex.x, pathTileIndex.y, eTileCategory::Ground);
			}
		},
		[this](std::unique_ptr<RoomSpace> roomSpace)
		{
			//방 객체 생성 및 타일 구성
			UNIQUE_INDEX_TYPE newRoomIndex = static_cast<UNIQUE_INDEX_TYPE>(mapRooms.size() + 1);

			std::unique_ptr<Room> newRoom = std::make_unique<Room>(*this, newRoomIndex, std::move(roomSpace));
			assert(newRoom && "Invalid newRoom");

			//방 안의 타일들 생성
			const RoomSpace::RoomTileIndices& newRoomInnerTiles = newRoom->GetRoomSpace().GetInnerTileIndices();
			for (const Vector2Int& tileIndex : newRoomInnerTiles)
			{
				SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
			}

			mapRooms.insert(std::pair<UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>(newRoomIndex, std::move(newRoom)));
			newRoom.reset();
		}
	);
}

void Tilemap::Tick(float deltaTime)
{
	
}

void Tilemap::Draw()
{
	for (const auto& iterTile : tileList)
	{
		Color TileColor;
		std::string TileSprite;
		switch (iterTile->GetTileCategory())
		{
		case eTileCategory::Wall:
			TileColor = Color::White;
			TileSprite = "#";
			break;

		case eTileCategory::Ground:
			TileColor = Color::White;
			TileSprite = " ";
			break;

		default:
			TileColor = Color::White;
			TileSprite = "e";
			break;
		}

		Renderer::Get().Submit(TileSprite, iterTile->GetTilePosition(), TileColor );
	}
}

void Tilemap::SetTileCategory(int xPos, int yPos, eTileCategory category)
{
	const size_t TileIndex = (yPos * mapSize.x) + xPos;
	if (TileIndex < 0 || TileIndex >= tileList.size())
	{
		return;
	}

	if (!tileList[TileIndex])
	{
		return;
	}

	tileList[TileIndex]->SetTileCategory(eTileCategory::Ground);
}