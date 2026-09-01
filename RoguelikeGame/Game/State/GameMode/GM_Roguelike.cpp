#include "GM_Roguelike.h"
#include "Util/Util.h"
#include "Level/GameLevel.h"
#include "TileMap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Game/State/PlayerState/PS_Roguelike.h"
#include "Actor/MapObject/RoomDoor.h"
#include "Actor/MapObject/PlayerStart.h"
#include "Actor/MapObject/NextLevel.h"
#include "Actor/MapObject/Exit.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Actor/Pawn/NPC/Slime/NPCSlime.h"
#include "Actor/Pawn/NPC/GoblinArcher/NPCGoblinArcher.h"
#include "Actor/Pawn/NPC/Orc/NPCOrc.h"
#include "Actor/Pawn/NPC/Troll/NPCTroll.h"
#include "Actor/Pawn/NPC/Golem/NPCGolem.h"
#include "Actor/Pawn/NPC/Imp/NPCImp.h"
#include "Actor/Pawn/NPC/Sharman/NPCSharman.h"
#include "Actor/FieldItem/HealthPotion.h"
#include "Actor/FieldItem/FieldSkillItem.h"
#include <Engine/Engine.h>
#include <Math/Vector2Int.h>
#include <cassert>

using namespace Craft;

GM_Roguelike::GM_Roguelike()
{

}

GM_Roguelike::~GM_Roguelike()
{

}

void GM_Roguelike::OnInitializeLevel(std::weak_ptr<Craft::Level> level)
{
	super::OnInitializeLevel(level);

	if (std::shared_ptr<TilemapLevel> currentTileMap = GetCurrentLevel<TilemapLevel>())
	{
		/* 타일맵 레벨 내에서 발생하는 이벤트들을 대기한다. */
		currentTileMap->SetPlayerVisitedRoomEventCallback(std::bind(&GM_Roguelike::OnPlayerVisitedRoom,
			this,
			std::placeholders::_1,
			std::placeholders::_2));

		currentTileMap->SetPlayerLeavedRoomEventCallback(std::bind(&GM_Roguelike::OnPlayerLeavedRoom,
			this,
			std::placeholders::_1,
			std::placeholders::_2));
	}

	ReadyGameActorSpawn();
	PlayerPawnSpawn();

	IncrementFloorLevel();

	/*switch (currentFloorLevel)
	{
	case 1:
		Engine::Get().PlayBackgroundMusic("BGM/stage1.wav");
		break;

	case 2:
		Engine::Get().PlayBackgroundMusic("BGM/stage2.wav");
		break;

	default:
		Engine::Get().PlayBackgroundMusic("BGM/stage3.wav");
		break;
	}*/
	
}

void GM_Roguelike::OnDestroyedCurrentLevel()
{
	//레벨 초기화 전 레벨 안에서 참조하던 액터 및 진행 정보를 모두 초기화해야함
	spawnedPlayerStart.reset();
	playerPawn.reset();
	playerVisitedRoomIndices.clear();

	currentPlayerVisitRoomIndex = RoomDefines::ROOM_INDEX_INVALID;
	currentPlayerVisitRoomType = eRoomType::None;
	spawnedNPCList.clear();
	spawnedRoomDoors.clear();

	super::OnDestroyedCurrentLevel();
}

const Craft::Vector2Int& GM_Roguelike::GetNextFloorRoomDoorPosition() const
{
	return nextFloorRoomDoorPos;
}

std::unique_ptr<PlayerState> GM_Roguelike::CreatePlayerState() const
{
	return std::make_unique<PS_Roguelike>();
}

void GM_Roguelike::ReadyGameActorSpawn()
{
	std::shared_ptr<TilemapLevel> level = GetCurrentLevel<TilemapLevel>();
	assert(level && "Invalid level");

	/* 생성된 방 별로 호출되는 콜백 람다 */
	auto callbackRooms = [&](const Room& room)
		{
			const RoomSpace& roomSpace = room.GetRoomSpace();

			switch (room.GetRoomType())
			{
			case eRoomType::Start:
				{
					//플레이어의 시작 위치 오브젝트 생성
					const Vector2Int& selectTilePos = roomSpace.GetPositionCenter();
					spawnedPlayerStart = level->SpawnActor<PlayerStart>(selectTilePos);
				}
				break;

			case eRoomType::NextLevel:
				{
					//입구 타일들 중 첫번째 타일을 지정
					nextFloorRoomDoorPos = *roomSpace.GetDoorTileIndices().begin();
				}
				break;

			case eRoomType::Exit:
				{
					//출구 오브젝트 랜덤한 위치에 생성
					const Vector2Int& selectTilePos = roomSpace.GetPositionCenter();
					level->SpawnActor<Exit>(selectTilePos);
				}
				break;
			}
		};

	level->Foreach_Rooms(callbackRooms);
}

void GM_Roguelike::PlayerPawnSpawn()
{
	std::shared_ptr<TilemapLevel> level = GetCurrentLevel<TilemapLevel>();
	assert(level && "Invalid level");

	std::shared_ptr<PlayerStart> playerStart = spawnedPlayerStart.lock();
	assert(playerStart && "playerStart Invalid..");

	std::shared_ptr<PlayerPawn> spawnedPlayerPawn = level->SpawnActor<PlayerPawn>(playerStart->GetWorldPosition());
	assert(spawnedPlayerPawn && "spawn playerPawn fail..");
	spawnedPlayerPawn->SetDeathEventCallback(std::bind(&GM_Roguelike::OnEventPlayerDeath, this, std::placeholders::_1));
	playerPawn = spawnedPlayerPawn;

	// 플레이어가 최조 스폰된 방에 대한 이벤트 처리
	const Vector2Int playerPosition = spawnedPlayerPawn->GetWorldPosition();
	const Room* spawnedRoom = level->GetPostionInRoom(spawnedPlayerPawn->GetWorldPosition());
	assert(spawnedRoom && "Spawn room Invalid");
	OnPlayerVisitedRoom(*spawnedRoom, playerPosition);

	/* PlayerState에 알림 */
	PS_Roguelike* currentPlayerState = GetPlayerState<PS_Roguelike>();
	assert(currentPlayerState && "currentPlayerState Invalid");

	currentPlayerState->OnSpawnedPlayerPawn(playerPawn);
}

void GM_Roguelike::IncrementFloorLevel()
{
	++currentFloorLevel;

	PS_Roguelike* playerState = GetPlayerState<PS_Roguelike>();
	if (playerState)
	{
		playerState->ChangeFloorLevel(currentFloorLevel);
	}
}

void GM_Roguelike::OnPlayerVisitedRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition)
{
	currentPlayerVisitRoomIndex = visitRoom.GetRoomUniqueIndex();
	currentPlayerVisitRoomType = visitRoom.GetRoomType();

	//이미 방문한 적이 있으면 이벤트 진행 x
	if (playerVisitedRoomIndices.end() != playerVisitedRoomIndices.find(currentPlayerVisitRoomIndex))
	{
		return;
	}

	switch (currentPlayerVisitRoomType)
	{
	case eRoomType::Battle:
		{
			OnPlayerVisitedBattleRoom(visitRoom, playerPosition);
		}
		break;

	case eRoomType::Treasure:
		{
			OnPlayerVisitedTreasureRoom(visitRoom, playerPosition);
		}
		break;

	case eRoomType::NextLevel:
		{
			OnPlayerVisitedNextRoom(visitRoom, playerPosition);
		}
		break;
	}

	// 방문한 방 인덱스 추가
	playerVisitedRoomIndices.insert(currentPlayerVisitRoomIndex);
}

void GM_Roguelike::OnPlayerLeavedRoom(const Room& leaveRoom, const Craft::Vector2Int& playerPosition)
{
	const RoomDefines::UNIQUE_INDEX_TYPE leaveRoomIndex = leaveRoom.GetRoomUniqueIndex();
	if (currentPlayerVisitRoomIndex == leaveRoomIndex)
	{
		currentPlayerVisitRoomIndex = RoomDefines::ROOM_INDEX_INVALID;
		currentPlayerVisitRoomType = eRoomType::None;
	}	
}

void GM_Roguelike::OnPlayerVisitedBattleRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition)
{
	std::shared_ptr<Level> level = GetCurrentLevel<Level>();
	if (!level)
	{
		return;
	}

	const RoomSpace& visitRoomSpace = visitRoom.GetRoomSpace();

	/* 방안의 랜덤한 위치에 몬스터들 생성 */
	const RoomSpace::RoomTileIndices spawnTileIndices = visitRoomSpace.GetInnerTileIndices();
	std::vector<size_t> shuffleSpawnIndex;
	for(size_t tileIndex = 0; tileIndex < spawnTileIndices.size(); ++tileIndex)
	{
		/* 플레이어가 위치한 Tile은 제외한다. */
		if (spawnTileIndices[tileIndex] == playerPosition)
		{
			continue;
		}

		shuffleSpawnIndex.emplace_back(tileIndex);
	}

	/* 현재 플레이어가 위치한 타일 갯수를 제외한 나머지 갯수 = 스폰 가능 지점 갯수 */
	const int maxSpawnNum = static_cast<int>(shuffleSpawnIndex.size());
	if (maxSpawnNum <= 0)
	{
		/* 방 안에 플레이어 제외 몬스터가 위치할 타일들이 존재하지 않으면 반환 */
		return;
	}

	/* Spawn Index 리스트를 뒤섞는다. */
	for (size_t i = shuffleSpawnIndex.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 방 인덱스와 랜덤하게 결정된 방 인덱스를 교환해서 뒤섞는다.
		std::swap(shuffleSpawnIndex[i], shuffleSpawnIndex[j]);
	}

	/* 스폰할 몬스터 수 결정 */
	const int spawnMinRange = min(15, maxSpawnNum);
	const int spawnMaxRange = min(30, maxSpawnNum);
	int spawnMonsterNum = Util::RandomRange(spawnMinRange, spawnMaxRange);
	/* 테스트용 */
	spawnMonsterNum = 30;
	int currentIndex = 0;

	auto spawnRandomNPC = [this, level](const Vector2Int& spawnTilePos, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
		{
			const int randomValue = Util::RandomRange(0, 99);

			std::shared_ptr<NPCBase> spawnedNPC = nullptr;

			switch (currentFloorLevel)
			{
			case 1:
				/*if (randomValue < 30)
				{
					spawnedNPC = level->SpawnActor<NPCGoblinArcher>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 80)
				{
					spawnedNPC = level->SpawnActor<NPCSlime>(spawnTilePos, roomIndex);
				}
				else
				{
					spawnedNPC = level->SpawnActor<NPCOrc>(spawnTilePos, roomIndex);
				}*/
				/* 테스트용 */
				spawnedNPC = level->SpawnActor<NPCSlime>(spawnTilePos, roomIndex);
				break;

			case 2:
				if (randomValue < 10)
				{
					spawnedNPC = level->SpawnActor<NPCGoblinArcher>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 25)
				{
					spawnedNPC = level->SpawnActor<NPCSlime>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 50)
				{
					spawnedNPC = level->SpawnActor<NPCOrc>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 65)
				{
					spawnedNPC = level->SpawnActor<NPCImp>(spawnTilePos, roomIndex);
				}
				else
				{
					spawnedNPC = level->SpawnActor<NPCTroll>(spawnTilePos, roomIndex);
				}
				break;

			default:
				if (randomValue < 10)
				{
					spawnedNPC = level->SpawnActor<NPCGoblinArcher>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 15)
				{
					spawnedNPC = level->SpawnActor<NPCSlime>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 30)
				{
					spawnedNPC = level->SpawnActor<NPCOrc>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 40)
				{
					spawnedNPC = level->SpawnActor<NPCImp>(spawnTilePos, roomIndex);
				}
				else if(randomValue < 60)
				{
					spawnedNPC = level->SpawnActor<NPCTroll>(spawnTilePos, roomIndex);
				}
				else if (randomValue < 75)
				{
					spawnedNPC = level->SpawnActor<NPCSharman>(spawnTilePos, roomIndex);
				}
				else
				{
					spawnedNPC = level->SpawnActor<NPCGolem>(spawnTilePos, roomIndex);
				}
				break;
			}	

			return spawnedNPC;
		};

	while (spawnMonsterNum > 0 && 
		currentIndex < static_cast<int>(shuffleSpawnIndex.size()))
	{
		const size_t spawnTileIndex = shuffleSpawnIndex[currentIndex++];
		const Vector2Int& spawnTilePos = spawnTileIndices[spawnTileIndex];

		//랜덤으로 결정된 위치에 몬스터 스폰
		std::shared_ptr<NPCBase> spawnedNPC = spawnRandomNPC(spawnTilePos, currentPlayerVisitRoomIndex);
		assert(spawnedNPC && "Spawn NPC Fail..");

		/* 생성된 NPC가 사망했을때의 이벤트 설정 */
		spawnedNPC->SetDeathEventCallback(std::bind(&GM_Roguelike::OnEventNPCDeath, this, std::placeholders::_1));

		spawnedNPCList.emplace_back(spawnedNPC);

		--spawnMonsterNum;
	}

	/* 방의 입구에 문 Actor 생성 */
	const RoomSpace::RoomDoorTileIndices& doorTileIndices = visitRoomSpace.GetDoorTileIndices();
	for (const Vector2Int& doorTileIndex : doorTileIndices)
	{
		std::shared_ptr<RoomDoor> spawnedRoomDoor = level->SpawnActor<RoomDoor>(doorTileIndex);
		assert(spawnedRoomDoor && "Spawn Door Actor Fail..");

		spawnedRoomDoors.emplace_back(spawnedRoomDoor);
	}

	/* 전투 진행여부 설정 */
	bBattleRoomProcess = true;

	/* 몬스터들에게 플레이어 추적 명령 내림 */
	for (std::shared_ptr<NPCBase>& SpawnNPC : spawnedNPCList)
	{
		SpawnNPC->SetChaseTarget(playerPawn);
	}
}

void GM_Roguelike::OnPlayerVisitedTreasureRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition)
{
	std::shared_ptr<Level> level = GetCurrentLevel<Level>();
	if (!level)
	{
		return;
	}

	const RoomSpace& visitRoomSpace = visitRoom.GetRoomSpace();

	/* 방안의 랜덤한 위치에 아이템들 생성 */
	const RoomSpace::RoomTileIndices spawnTileIndices = visitRoomSpace.GetInnerTileIndices();
	std::vector<size_t> shuffleSpawnIndex;
	for (size_t tileIndex = 0; tileIndex < spawnTileIndices.size(); ++tileIndex)
	{
		/* 플레이어가 위치한 Tile은 제외한다. */
		if (spawnTileIndices[tileIndex] == playerPosition)
		{
			continue;
		}

		shuffleSpawnIndex.emplace_back(tileIndex);
	}

	/* 현재 플레이어가 위치한 타일 갯수를 제외한 나머지 갯수 = 스폰 가능 지점 갯수 */
	const int maxSpawnNum = static_cast<int>(shuffleSpawnIndex.size());
	if (maxSpawnNum <= 0)
	{
		/* 방 안에 플레이어 제외 아이템이 위치할 타일들이 존재하지 않으면 반환 */
		return;
	}

	/* Spawn Index 리스트를 뒤섞는다. */
	for (size_t i = shuffleSpawnIndex.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 방 인덱스와 랜덤하게 결정된 방 인덱스를 교환해서 뒤섞는다.
		std::swap(shuffleSpawnIndex[i], shuffleSpawnIndex[j]);
	}

	/* 스폰할 아이템 갯수 결정 */
	const int spawnMinRange = min(3, maxSpawnNum);
	const int spawnMaxRange = min(6, maxSpawnNum);
	int spawnItemNum = Util::RandomRange(spawnMinRange, spawnMaxRange);
	int currentIndex = 0;

	auto spawnRandomItem = [level](const Vector2Int& spawnTilePos, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
		{
			const int randomValue = Util::RandomRange(0, 99);

			std::shared_ptr<FieldItem> spawnedItem = nullptr;

			if (randomValue < 20)
			{
				spawnedItem = level->SpawnActor<FieldSkillItem>(spawnTilePos, 1);
			}
			else if (randomValue < 40)
			{
				spawnedItem = level->SpawnActor<FieldSkillItem>(spawnTilePos, 2);
			}
			else
			{
				spawnedItem = level->SpawnActor<HealthPotion>(spawnTilePos, Util::RandomRange(1.f, 10.f));
			}

			return spawnedItem;
		};

	while (spawnItemNum > 0 &&
		currentIndex < static_cast<int>(shuffleSpawnIndex.size()))
	{
		const size_t spawnTileIndex = shuffleSpawnIndex[currentIndex++];
		const Vector2Int& spawnTilePos = spawnTileIndices[spawnTileIndex];

		//랜덤으로 결정된 위치에 아이템 스폰
		std::shared_ptr<FieldItem> spawnedItem = spawnRandomItem(spawnTilePos, currentPlayerVisitRoomIndex);
		assert(spawnedItem && "Spawn Item Fail..");

		--spawnItemNum;
	}
}

void GM_Roguelike::OnPlayerVisitedNextRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition)
{
	std::shared_ptr<Level> level = GetCurrentLevel<Level>();
	if (!level)
	{
		return;
	}

	const RoomSpace& visitRoomSpace = visitRoom.GetRoomSpace();

	//다음 층으로 이동할 입구 오브젝트 생성
	const Vector2Int& selectTilePos = visitRoomSpace.GetPositionCenter();
	level->SpawnActor<NextLevel>(selectTilePos);
}

void GM_Roguelike::OnRoomBattleEnd()
{
	/* 생성되어있던 문 Actor Destroy */
	for (std::shared_ptr<RoomDoor> roomDoor : spawnedRoomDoors)
	{
		if (!roomDoor)
		{
			continue;
		}

		roomDoor->Destroy();
	}
	spawnedRoomDoors.clear();

	bBattleRoomProcess = false;
}

void GM_Roguelike::OnEventNPCDeath(std::shared_ptr<Pawn> deathPawn)
{
	std::shared_ptr<NPCBase> deathNPC = Cast<NPCBase>(deathPawn);
	assert(deathNPC && "Invalid deathNPC");

	auto iterFindDeathpawn = std::find(spawnedNPCList.begin(), spawnedNPCList.end(), deathPawn);
	if(iterFindDeathpawn != spawnedNPCList.end())
	{
		spawnedNPCList.erase(iterFindDeathpawn);
		if (spawnedNPCList.empty())
		{
			/* 모든 NPC가 죽었으면 현재 방에서의 전투가 끝났다고 간주 */
			OnRoomBattleEnd();
		}
	}

	/* 플레이어의 킬 수 증가 */
	PS_Roguelike* playerState = GetPlayerState<PS_Roguelike>();
	if (playerState)
	{
		playerState->IncrementMonsterKillNum();
	}
}

void GM_Roguelike::OnEventPlayerDeath(std::shared_ptr<Pawn> deathPawn)
{
	std::shared_ptr<GameLevel> gameLevel = GetCurrentLevel<GameLevel>();
	if (gameLevel)
	{
		PS_Roguelike* playerState = GetPlayerState<PS_Roguelike>();
		assert(playerState && "Invalid playerState");

		gameLevel->OnPlayerDeath(*playerState);

		Engine::Get().PlayBackgroundMusic("BGM/defeat.wav");
	}
}
