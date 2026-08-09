#include "TilemapLevel.h"
#include "Engine/Engine.h"
#include "Game/Config/Config.h"
#include "Render/Renderer.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Util/Util.h"
#include "Actor/MapObject/PlayerStart.h"
#include "Actor/MapObject/NextLevel.h"
#include "Actor/MapObject/Exit.h"
#include "Actor/Pawn/PlayerPawn.h"
#include <cassert>
#include <Memory>

using namespace Craft;

TilemapLevel::TilemapLevel()
{

}

TilemapLevel::~TilemapLevel()
{

}

void TilemapLevel::OnInitialized()
{
	Level::OnInitialized();

	/* BSP로 타일맵 구성 */
	BuildTilemapBSP();

	/* 만들어진 방들의 종류 지정 */
	AssignRoomType();

	/* 게임 시작 전 필요한 액터 생성 */
	ReadyGame();

	/* 플레이어 생성 및 게임 시작 */
	PlayerPawnSpawn();
}

void TilemapLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	if (tileMap)
	{
		tileMap->Tick(deltaTime);
	}
}

void TilemapLevel::Draw()
{
	if (tileMap)
	{
		tileMap->Draw();
	}

	Level::Draw();
}

bool TilemapLevel::CanNextMove(const Actor& checkActor, const Vector2Float& nextPosition)
{
	const eTileCategory nextTileCategory = tileMap->GetTileCategory(nextPosition);
	return eTileCategory::Ground == nextTileCategory;
}

void TilemapLevel::BuildTilemapBSP()
{
	//이전 타일맵 정보 초기화
	tileMap.reset();

	//새로운 타일맵 생성
	tileMap = std::make_unique<Tilemap>(*this);
	assert(tileMap && "Fail Tilemap Alloc..");

	auto lambdaOpenPath = [this](const std::vector<Craft::Vector2Int>& pathTileIndices)
		{
			for (const Vector2Int& pathTileIndex : pathTileIndices)
			{
				//경로상의 타일 뚫기
				tileMap->SetTileCategory(pathTileIndex.x, pathTileIndex.y, eTileCategory::Ground);
			}
		};

	auto lambdaGenerateRoom = [this](std::unique_ptr<RoomSpace> roomSpace)
		{
			//방 객체 생성 및 타일 구성
			UNIQUE_INDEX_TYPE newRoomIndex = static_cast<UNIQUE_INDEX_TYPE>(mapRooms.size() + 1);

			std::unique_ptr<Room> newRoom = std::make_unique<Room>(newRoomIndex, std::move(roomSpace));
			assert(newRoom && "Invalid newRoom");

			//방 안의 타일들 생성
			const RoomSpace::RoomTileIndices& newRoomInnerTiles = newRoom->GetRoomSpace().GetInnerTileIndices();
			for (const Vector2Int& tileIndex : newRoomInnerTiles)
			{
				tileMap->SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
			}

			mapRooms.insert(std::pair<UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>(newRoomIndex, std::move(newRoom)));
			newRoom.reset();
		};

	const Config& config = Engine::Get().GetConfig<Config>();
	const Vector2Int tilemapSize(config.GetTilemapWidth(), config.GetTilemapHeight());
	tileMap->InitializeTilemap(tilemapSize, lambdaOpenPath, lambdaGenerateRoom);
}

void TilemapLevel::AssignRoomType()
{
	//입구, 출구 등 방 종류들을 랜덤하게 선택한다.
	std::vector<RoomDefines::UNIQUE_INDEX_TYPE> roomKeys;
	for (const auto& pairRoom : mapRooms)
	{
		roomKeys.push_back(pairRoom.first);
	}

	//셔플 알고리즘으로 방의 키값들을 뒤섞는다.
	assert(!roomKeys.empty() && "roomKeys empty");
	for (size_t i = roomKeys.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 방 인덱스와 랜덤하게 결정된 방 인덱스를 교환해서 뒤섞는다.
		std::swap(roomKeys[i], roomKeys[j]);
	}

	//앞의 2개는 각각 방의 시작지점과 다음레벨로 지정한다.
	//TODO : 마지막 층이면 다음레벨대신 출구를 지정해야 한다.
	mapRooms[roomKeys[0]]->SetRoomType(eRoomType::Start);
	mapRooms[roomKeys[1]]->SetRoomType(eRoomType::NextLevel);

	const float battleRoomRatio = 0.7f;

	//나머지 방들은 비율에 맞춰 보물창고 / 전투 타입으로 지정한다.
	const auto& middleRoomStart = roomKeys.begin() + 2;
	const auto& middleRoomEnd = roomKeys.end();
	const int middleRoomLength = static_cast<int>(middleRoomEnd - middleRoomStart);
	const auto& treasureRoomStart = middleRoomStart + static_cast<int>(static_cast<float>(middleRoomLength) * battleRoomRatio);

	//전투 방 지정
	for (auto iterBattleRoom = middleRoomStart; iterBattleRoom != treasureRoomStart; ++iterBattleRoom)
	{
		mapRooms[*iterBattleRoom]->SetRoomType(eRoomType::Battle);
	}

	//아이템 방 지정
	for (auto iterTreasureRoom = treasureRoomStart; iterTreasureRoom != middleRoomEnd; ++iterTreasureRoom)
	{
		mapRooms[*iterTreasureRoom]->SetRoomType(eRoomType::Treasure);
	}
}

void TilemapLevel::ReadyGame()
{
	for (const auto& pairRoom : mapRooms)
	{
		if (!pairRoom.second)
		{
			continue;
		}

		const Room& room = *pairRoom.second;
		const RoomSpace& roomSpace = room.GetRoomSpace();

		switch (pairRoom.second->GetRoomType())
		{
		case eRoomType::Start:
			{
				//플레이어의 시작 위치 오브젝트 생성
				const Vector2Float selectTilePos = static_cast<Vector2Float>(roomSpace.GetPositionCenter());
				spawnedPlayerStart = SpawnActor<PlayerStart>(selectTilePos);
			}
			break;

		case eRoomType::NextLevel:
			{
				//다음 층으로 이동할 입구 오브젝트 생성
				const Vector2Float selectTilePos = static_cast<Vector2Float>(roomSpace.GetPositionCenter());
				SpawnActor<NextLevel>(selectTilePos);
			}
			break;

		case eRoomType::Exit:
			{
				//출구 오브젝트 랜덤한 위치에 생성
				const Vector2Float selectTilePos = static_cast<Vector2Float>(roomSpace.GetPositionCenter());
				SpawnActor<Exit>(selectTilePos);
			}
			break;

		case eRoomType::Treasure:
			{
				//랜덤하게 아이템 오브젝트 생성
			}
			break;

		case eRoomType::Battle:
			{
				//랜덤하게 생성할 몬스터의 위치 결정
			}
			break;

		default:
			{

			}
			break;
		}
	}
}

void TilemapLevel::PlayerPawnSpawn()
{
	PlayerStart* playerStart = spawnedPlayerStart.get();
	assert(playerStart && "playerStart Invalid..");
	
	playerPawn = SpawnActor<PlayerPawn>(playerStart->GetWorldPosition());
}
