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
	char szTmp[256] = { 0 };

	//std::cout << "DeltaTime: " << deltaTime << " | FPS: " << (1.f / deltaTime) << '\n';
	sprintf_s(szTmp, "DeltaTime: %f, FPS : %f\n", deltaTime, (1.f / deltaTime));
	OutputDebugStringA(szTmp);
}

void Tilemap::Draw()
{
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

			Color TileColor;
			std::string TileSprite;
			switch (tile.GetTileCategory())
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

			const Vector2Float tilePosition = static_cast<Vector2Float>(tile.GetTilePosition());
			Renderer::Get().Submit(TileSprite, tilePosition, TileColor);
		}
	}


	/*for (const auto& iterTile : tileList)
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

		const Vector2Float tilePosition = static_cast<Vector2Float>(iterTile->GetTilePosition());
		Renderer::Get().Submit(TileSprite, tilePosition, TileColor);
	}*/
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

eTileCategory Tilemap::GetTileCategory(const Craft::Vector2Float& position) const
{
	const Vector2Int tile2dIndex = static_cast<Vector2Int>(position);
	const int tileIndex = (tile2dIndex.y * mapSize.x) + tile2dIndex.x;

	if (tileIndex >= 0 && tileIndex < tileList.size())
	{
		return tileList[tileIndex]->GetTileCategory();
	}
	
	return eTileCategory::None;
}
