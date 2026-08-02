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

	BSPRoot->ForeachNode([this](const BSPNode& Node)
		{
			switch (Node.GetNodeCategory())
			{
			case BSPNode::eNodeCategory::Corridor:
				{
					//TODO : 통로 타일 뚫기
				}
				break;

			case BSPNode::eNodeCategory::Room:
				{
					const Vector2& StartPosition = Node.GetStartPosition() + Vector2(1, 1);
					const int RoomWidth = Node.GetWidth() - 2;
					const int RoomHeight = Node.GetHeight() - 2;
					for (int y = 0; y < RoomHeight; ++y)
					{
						const int TilePosY = StartPosition.y + y;
						
						for (int x = 0; x < RoomWidth; ++x)
						{
							const int TilePosX = StartPosition.x + x;

							//방 안의 공간 타일 뚫기
							SetTileCategory(TilePosX, TilePosY, eTileCategory::Ground);
						}
					}
				}
				break;
			}
		});
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
