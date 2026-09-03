#include "Tilemap.h"
#include "Engine/Engine.h"
#include "Game/Config/Config.h"
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
	const CameraManager& cameraManager = Engine::Get().GetCameraManager();

	/* 맵 외곽 가로 길이*/
	const int cameraViewWidth = cameraManager.GetViewWidth();
	const int mapBoundaryWidth = cameraViewWidth >> 1;

	/* 맵 외곽 세로 길이*/
	const int cameraViewHeight = cameraManager.GetViewHeight();
	const int mapBoundaryHeight = cameraViewHeight >> 1;

	/* 맵의 사이즈 (지정된 맵사이즈 + 카메라가 최대 볼 수 있는 외곽영역 포함 사이즈 )*/
	mapSize.x = inMapSize.x + cameraViewWidth;
	mapSize.y = inMapSize.y + cameraViewHeight;

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
	leftTopPos.x = mapBoundaryWidth;
	leftTopPos.y = mapBoundaryHeight;
	innerTileRect.x = mapSize.x - cameraViewWidth;
	innerTileRect.y = mapSize.y - cameraViewHeight;
	std::unique_ptr<BSPNode> BSPRoot = std::make_unique<BSPNode>(leftTopPos.x, leftTopPos.y, innerTileRect.x, innerTileRect.y);
	assert(BSPRoot && "BSPRoot Alloc Failed..");
	BSPRoot->Divide();

	//분할된 공간을 연결
	BSPRoot->ConnectRooms();

	//방, 경로 정보들을 바탕으로 Tile 구성
	BSPRoot->ExtractNodeContents(CorridorCallback, RoomCallback);

	//현재 카메라가 표시하는 화면의 길이만큼 타일맵이 한줄씩 표시되므로 DrawBuffer의 크기를 미리 지정해둔다.
	drawTileLineBuffer.reserve(cameraViewWidth);
}

void Tilemap::InitializeTilemapBossRoom(const Vector2Int& inMapSize, 
										std::function<void(const std::vector<Craft::Vector2Int>&)> CorridorCallback, 
										std::function<void(std::unique_ptr<RoomSpace>, bool)> RoomCallback)
{
	const Config& config = Engine::Get().GetConfig<Config>();
	const CameraManager& cameraManager = Engine::Get().GetCameraManager();

	/* 맵 외곽 가로 길이*/
	const int cameraViewWidth = cameraManager.GetViewWidth();
	const int mapBoundaryWidth = cameraViewWidth >> 1;

	/* 맵 외곽 세로 길이*/
	const int cameraViewHeight = cameraManager.GetViewHeight();
	const int mapBoundaryHeight = cameraViewHeight >> 1;

	/* 맵의 사이즈 (지정된 맵사이즈 + 카메라가 최대 볼 수 있는 외곽영역 포함 사이즈 )*/
	mapSize.x = inMapSize.x + cameraViewWidth;
	mapSize.y = inMapSize.y + cameraViewHeight;

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

	leftTopPos.x = mapBoundaryWidth;
	leftTopPos.y = mapBoundaryHeight;
	innerTileRect.x = mapSize.x - cameraViewWidth;
	innerTileRect.y = mapSize.y - cameraViewHeight;

	//방 외곽 벽 두께
	const int wallThickness = config.GetBSPRoomWallThickness();

	auto generateRoomSpace = [&](int roomWidth, int roomHeight, const Vector2Int& roomStart)
		{
			const int roomOuterWidth = roomWidth + (wallThickness << 1);
			const int roomOuterHeight = roomHeight + (wallThickness << 1);
			const Vector2Int roomOuterStart(roomStart - Vector2Int(wallThickness, wallThickness));

			std::unique_ptr<RoomSpace> roomSpace = std::make_unique<RoomSpace>(roomStart, roomWidth, roomHeight,
																				roomOuterStart, roomOuterWidth, roomOuterHeight);
			assert(roomSpace);
			roomSpace->InitializeRoomSpace();

			return roomSpace;
		};

	//보스방 생성
	static const int bossRoomWidth = 200;
	static const int bossRoomHeight = 60;

	const Vector2Int bossRoomStart(leftTopPos.x + (innerTileRect.x >> 1) - (bossRoomWidth >> 1), 
									leftTopPos.y + (innerTileRect.y >> 1) - (bossRoomHeight >> 1));

	std::unique_ptr<RoomSpace> generateBossRoomSpace = generateRoomSpace(bossRoomWidth, bossRoomHeight, bossRoomStart);
	assert(generateBossRoomSpace && "generate fail BossRoom Space");
	
	//보스방 밑 시작 방 공간 생성
	static const int startRoomWidth = 60;
	static const int startRoomHeight = 40;

	//시작방과 보스방을 연결하는 통로의 길이
	static const int corridorWidth = 4;
	static const int corridorHeight = 20;
	
	const Vector2Int startRoomStart(leftTopPos.x + (innerTileRect.x >> 1) - (startRoomWidth >> 1),
									bossRoomStart.y + bossRoomHeight + corridorHeight);

	std::unique_ptr<RoomSpace> generateStartRoomSpace = generateRoomSpace(startRoomWidth, startRoomHeight, startRoomStart);
	assert(generateStartRoomSpace && "generate fail StartRoom Space");


	//보스방과 시작방 연결 통로 생성
	const int startCorridorXPos = startRoomStart.x + (startRoomWidth >> 1) - (corridorWidth >> 1);
	const int endCorridorXPos = startCorridorXPos + corridorWidth;
	const int startCorridorYPos = bossRoomStart.y + bossRoomHeight;
	const int endCorridorYPos = startCorridorYPos + corridorHeight;

	std::vector<Vector2Int> corridorTiles;
	corridorTiles.reserve(corridorWidth * corridorHeight);
	for (int yPos = startCorridorYPos; yPos < endCorridorYPos; ++yPos)
	{
		for (int xPos = startCorridorXPos; xPos < endCorridorXPos; ++xPos)
		{
			corridorTiles.emplace_back(Vector2Int(xPos, yPos));
		}
	}

	// 보스방과 시작방 문 타일위치 저장.
	for (int xPos = startCorridorXPos; xPos < endCorridorXPos; ++xPos)
	{
		generateBossRoomSpace->AddDoorTile(eRoomSides::Bottom, Vector2Int(xPos, startCorridorYPos));
		generateStartRoomSpace->AddDoorTile(eRoomSides::Top, Vector2Int(xPos, endCorridorYPos - 1));
	}

	if (CorridorCallback)
	{
		CorridorCallback(corridorTiles);
	}
	
	assert(RoomCallback && "Invalid RoomCallback");
	RoomCallback(std::move(generateBossRoomSpace), true);

	assert(RoomCallback && "Invalid RoomCallback");
	RoomCallback(std::move(generateStartRoomSpace), false);
}

void Tilemap::Tick(float deltaTime)
{
	
}

void Tilemap::Draw()
{
	Renderer& renderer = Renderer::Get();

	//카메라에 표시되는 타일들만 출력한다. 
	const CameraManager& cameraManager = Engine::Get().GetCameraManager();
	const Vector2Int leftTopPosition = cameraManager.GetLeftTopPosition();
	const Vector2Int rightDownPosition = cameraManager.GetRightDownPosition();
	const int drawTileWidth = rightDownPosition.x - leftTopPosition.x;

	for (int y = leftTopPosition.y; y != rightDownPosition.y; ++y)
	{
		drawTileLineBuffer.clear();
		drawTileLineBuffer.resize(drawTileWidth);
		int tileBufferIndex = 0;

		for (int x = leftTopPosition.x; x != rightDownPosition.x; ++x, ++tileBufferIndex)
		{
			const size_t tileIndex = (y * mapSize.x) + x;
			if (tileIndex >= tileList.size())
			{
				continue;
			}

			const Tile& tile = *tileList[tileIndex];

			/* 벽타일과 방 입구 문 타일만 그린다. */
			switch (tile.GetTileCategory())
			{
			case eTileCategory::Wall:
				{
					drawTileLineBuffer[tileBufferIndex] = L'█';
				}
				break;

			case eTileCategory::Ground:
				{
					drawTileLineBuffer[tileBufferIndex] = L' ';
				}
				break;

			default:
				{
					drawTileLineBuffer[tileBufferIndex] = L'e';
				}
				break;
			}
		}

		// Buffer Submit;
		renderer.Submit(drawTileLineBuffer, Vector2Int(leftTopPosition.x, y), Color::DarkGray);
	}







	//카메라에 표시되는 타일들만 출력한다. 
	//const CameraManager& cameraManager = Engine::Get().GetCameraManager();
	//const Vector2Int leftTopPosition = cameraManager.GetLeftTopPosition();
	//const Vector2Int rightDownPosition = cameraManager.GetRightDownPosition();

	//for (int y = leftTopPosition.y; y != rightDownPosition.y; ++y)
	//{
	//	for (int x = leftTopPosition.x; x != rightDownPosition.x; ++x)
	//	{
	//		const size_t tileIndex = (y * mapSize.x) + x;
	//		if (tileIndex < 0 || tileIndex >= tileList.size())
	//		{
	//			continue;
	//		}

	//		const Tile& tile = *tileList[tileIndex];

	//		/* 벽타일과 방 입구 문 타일만 그린다. */
	//		switch (tile.GetTileCategory())
	//		{
	//		case eTileCategory::Wall:
	//			{
	//				Renderer::Get().Submit(L"█", tile.GetTilePosition(), Color::DarkGray);
	//			}
	//			break;
	//		}

	//		/*Color TileColor;
	//		std::wstring TileSprite;

	//		switch (tile.GetTileCategory())
	//		{
	//		case eTileCategory::Wall:
	//			TileColor = Color::DarkGray;
	//			TileSprite = L"█";
	//			break;

	//		case eTileCategory::Ground:
	//			TileColor = Color::White;
	//			TileSprite = L" ";
	//			break;

	//		default:
	//			TileColor = Color::White;
	//			TileSprite = L"e";
	//			break;
	//		}

	//		Renderer::Get().Submit(TileSprite, tile.GetTilePosition(), TileColor);*/
	//	}
	//}
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
	if (TileIndex >= tileList.size())
	{
		return nullptr;
	}

	return tileList[TileIndex].get();
}
