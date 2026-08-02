#include "Tilemap.h"
#include "Render/Renderer.h"
#include "Tile.h"
#include "BSP/BSPNode.h"
#include "Util/Util.h"
#include <cassert>

using namespace Craft;

Tilemap::Tilemap()
{
	
}

Tilemap::~Tilemap()
{

}

void Tilemap::InitializeTilemap(const Vector2& inMapSize)
{
	mapSize = inMapSize;

	//생성할 타일 갯수
	const size_t GenerateMapTileNum = mapSize.x * mapSize.y;

	//벡터 공간 미리 확보
	TileList.reserve(GenerateMapTileNum);

	for (size_t TileIndex = 0; TileIndex < GenerateMapTileNum; ++TileIndex)
	{
		const int PosX = static_cast<int>(TileIndex % mapSize.x);
		const int PosY = static_cast<int>(TileIndex / mapSize.x);

		TileList.emplace_back(std::make_unique<Tile>(eTileCategory::Wall, Vector2(PosX, PosY), TileIndex));
	}

	//BSP알고리즘을 통한 공간 분할 및 방 생성
	BSPRoot = std::make_unique<BSPNode>(0, 0, mapSize.x, mapSize.y);
	assert(BSPRoot && "BSPRoot Alloc Failed..");
	BSPRoot->Divide();


	/*const int randomX = Util::RandomRange(0, mapSize.x - 1);
	const int randomY = Util::RandomRange(0, mapSize.y - 1);

	SetTileCategory(randomX, randomY, eTileCategory::Ground);*/
}

void Tilemap::Draw()
{
	for (const auto& iterTile : TileList)
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
	if (TileIndex < 0 || TileIndex >= TileList.size())
	{
		return;
	}

	if (!TileList[TileIndex])
	{
		return;
	}

	TileList[TileIndex]->SetTileCategory(eTileCategory::Ground);
}
