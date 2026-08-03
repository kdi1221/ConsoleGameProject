#include "Tilemap.h"
#include "Render/Renderer.h"
#include "Tile.h"
#include "BSP/BSPNode.h"
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

void Tilemap::InitializeTilemap(const Vector2& inMapSize)
{
	mapSize = inMapSize;

	//생성할 타일 갯수
	const size_t GenerateMapTileNum = mapSize.x * mapSize.y;

	//벡터 공간 미리 확보
	TileList.reserve(GenerateMapTileNum);

	//맵 내의 타일 초기화 및 벽으로 채우기
	for (size_t TileIndex = 0; TileIndex < GenerateMapTileNum; ++TileIndex)
	{
		const int PosX = static_cast<int>(TileIndex % mapSize.x);
		const int PosY = static_cast<int>(TileIndex / mapSize.x);

		TileList.emplace_back(std::make_unique<Tile>(eTileCategory::Wall, Vector2(PosX, PosY), TileIndex));
	}

	//BSP알고리즘을 통한 공간 분할 및 방 생성, 경로 정보 생성
	BSPRoot = std::make_unique<BSPNode>(0, 0, mapSize.x, mapSize.y);
	assert(BSPRoot && "BSPRoot Alloc Failed..");
	BSPRoot->Divide();

	//분할된 공간을 연결
	BSPRoot->GeneratePaths();

	//방, 경로 정보들을 바탕으로 Tile 구성
	BSPRoot->Foreach_Node([this](const BSPNode& node)
		{
			switch (node.GetNodeCategory())
			{
			case BSPNode::eNodeCategory::Corridor:
				{
					const std::vector<Craft::Vector2>& nodePaths = node.GetPaths();
					for (const Vector2& tileIndex : nodePaths)
					{
						//경로상의 타일 뚫기
						SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
					}
				}
				break;

			case BSPNode::eNodeCategory::Room:
				{
					const Room& nodeRoom = node.GetRoom();
					nodeRoom.Foreach_Tile([this](const Vector2& tileIndex)
					  {
						  //방 안의 공간 타일 뚫기
						  SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
					  });
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

//void Tilemap::GenerateTiles()
//{


	//for (const auto& room : rooms)
	//{
	//	assert(room);

	//	room->Foreach_Tile([this](const Vector2& tileIndex)
	//		{
	//			//방 안의 공간 타일 뚫기
	//			SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
	//		});
	//}
//}

//void Tilemap::GeneratePathTile()
//{
//	static const auto& GetAddValue = [](int Length)
//	{
//		if (Length == 0)
//		{
//			return 0;
//		}
//		else
//		{
//			return Length > 0 ? 1 : -1;
//		}
//	};
//
//	const size_t maxLoop = rooms.size() - 1;
//	for (size_t i = 0; i < maxLoop; ++i)
//	{
//		//현재 방과 다음 방 사이를 연결한다.
//		const Room& currentRoom = *rooms[i];
//		const Room& nextRoom = *rooms[i + 1];
//
//		const Vector2& currentRoomCenter = currentRoom.GetPositionCenter();
//		const Vector2& nextRoomCenter = nextRoom.GetPositionCenter();
//
//		const int widthCurrenttoNext = nextRoomCenter.x - currentRoomCenter.x;
//		const int heightCurrenttoNext = nextRoomCenter.y - currentRoomCenter.y;
//
//		const int addX = GetAddValue(widthCurrenttoNext);
//		const int addY = GetAddValue(heightCurrenttoNext);
//
//		const int absWidth = abs(widthCurrenttoNext);
//		const int absHeight = abs(heightCurrenttoNext);
//
//		int xPos = currentRoomCenter.x;
//		int yPos = currentRoomCenter.y;
//
//		if (absWidth > absHeight)
//		{
//			//가로를 나눠서 경로 구성
//			const int absHalfWidth = absWidth >> 1;
//			
//			//첫 절반 경로 생성
//			const int DestinationXPos = currentRoomCenter.x + (absHalfWidth * addX);
//			for (; xPos != DestinationXPos; xPos += addX)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//
//			//상하 경로 생성
//			for (; yPos != nextRoomCenter.y; yPos += addY)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//
//			//나머지 절반 경로 생성
//			for (; xPos != nextRoomCenter.x; xPos += addX)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//		}
//		else
//		{
//			//세로를 나눠서 경로 구성
//			const int absHalfHeight = absHeight >> 1;
//
//			//첫 절반 경로 생성
//			const int DestinationYPos = currentRoomCenter.y + (absHalfHeight * addY);
//			for (; yPos != DestinationYPos; yPos += addY)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//
//			//좌우 경로 생성
//			for (; xPos != nextRoomCenter.x; xPos += addX)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//
//			//나머지 절반 경로 생성
//			for (; yPos != nextRoomCenter.y; yPos += addY)
//			{
//				//경로내의 타일 뚫기
//				SetTileCategory(xPos, yPos, eTileCategory::Ground);
//			}
//		}
//	}
//}
