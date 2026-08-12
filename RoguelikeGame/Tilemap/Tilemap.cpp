#include "Tilemap.h"
#include "Engine/Engine.h"
#include "Render/Renderer.h"
#include "Tile.h"
#include "BSP/BSPNode.h"
#include "BSP/RoomSpace/RoomSpace.h"
#include "Util/Util.h"
#include "Level/Level.h"
#include "Camera/CameraManager.h"
#include <cassert>

using namespace Craft;

Tilemap::Tilemap(Level& inOwnerLevel)
	:ownerLevel(inOwnerLevel)
{
	
}

Tilemap::~Tilemap()
{

}

void Tilemap::InitializeTilemap(const Vector2Int& inMapSize, 
								std::function<void(const std::vector<Craft::Vector2Int>&)> CorridorCallback,
								std::function<void(std::unique_ptr<RoomSpace>)> RoomCallback)
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
	BSPRoot->ExtractNodeContents(CorridorCallback, RoomCallback);
}

void Tilemap::Tick(float deltaTime)
{
	
}

void Tilemap::Draw()
{
	//카메라에 표시되는 타일들만 출력한다. 
	const CameraManager& cameraManager = Engine::Get().GetCameraManager();
	const Vector2Int leftTopPosition = cameraManager.GetLeftTopPosition();
	const Vector2Int rightDownPosition = cameraManager.GetRightDownPosition();

	for (int y = leftTopPosition.y; y != rightDownPosition.y; ++y)
	{
		for (int x = leftTopPosition.x; x != rightDownPosition.x; ++x)
		{
			const size_t tileIndex = (y * mapSize.x) + x;
			if (tileIndex < 0 || tileIndex >= tileList.size())
			{
				continue;
			}

			const Tile& tile = *tileList[tileIndex];

			/* 벽타일과 방 입구 문 타일만 그린다. */
			switch (tile.GetTileCategory())
			{
			case eTileCategory::Wall:
				{
					Renderer::Get().Submit(L"█", tile.GetTilePosition(), Color::DarkGray);
				}
				break;
			}

			/*Color TileColor;
			std::wstring TileSprite;

			switch (tile.GetTileCategory())
			{
			case eTileCategory::Wall:
				TileColor = Color::DarkGray;
				TileSprite = L"█";
				break;

			case eTileCategory::Ground:
				TileColor = Color::White;
				TileSprite = L" ";
				break;

			default:
				TileColor = Color::White;
				TileSprite = L"e";
				break;
			}

			Renderer::Get().Submit(TileSprite, tile.GetTilePosition(), TileColor);*/
		}
	}
}

void Tilemap::SetTileRoomIndex(int xPos, int yPos, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
{
	Tile* findTile = GetTile(xPos, yPos);
	if (!findTile)
	{
		return;
	}

	findTile->SetTileRoomIndex(roomIndex);
}

void Tilemap::SetTileCategory(int xPos, int yPos, eTileCategory category)
{
	Tile* findTile = GetTile(xPos, yPos);
	if (!findTile)
	{
		return;
	}

	findTile->SetTileCategory(category);
}

eTileCategory Tilemap::GetTileCategory(const Craft::Vector2Int& position) const
{
	if (Tile* findTile = GetTile(position.x, position.y))
	{
		return findTile->GetTileCategory();
	}
	
	return eTileCategory::None;
}

RoomDefines::UNIQUE_INDEX_TYPE Tilemap::GetTileRoomIndex(const Craft::Vector2Int& position) const
{
	if (Tile* findTile = GetTile(position.x, position.y))
	{
		return findTile->GetRoomIndex();
	}

	return RoomDefines::ROOM_INDEX_INVALID;
}

Tile* Tilemap::GetTile(int xPos, int yPos) const
{
	const size_t TileIndex = (yPos * mapSize.x) + xPos;
	if (TileIndex < 0 || TileIndex >= tileList.size())
	{
		return nullptr;
	}

	return tileList[TileIndex].get();
}
