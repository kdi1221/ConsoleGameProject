#include "GM_Roguelike.h"
#include "Engine/Engine.h"
#include "Util/Util.h"
#include "Level/TilemapLevel.h"
#include "TileMap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Actor/MapObject/RoomDoor.h"
#include "Actor/Pawn/NPC/NPCSlime.h"
#include <Math/Vector2Int.h>

using namespace Craft;

GM_Roguelike::GM_Roguelike()
{

}

GM_Roguelike::~GM_Roguelike()
{

}

void GM_Roguelike::SetCurrentLevel(std::weak_ptr<Level> level)
{
	super::SetCurrentLevel(level);


	if (std::shared_ptr<TilemapLevel> currentTileMap = GetCurrentLevel<TilemapLevel>())
	{
		/* 타일맵 레벨 내에서 발생하는 이벤트들을 대기한다. */
		currentTileMap->SetPlayerVisitedRoomEventCallback(std::bind(&GM_Roguelike::OnPlayerVisitedRoom, 
																	this, 
																	std::placeholders::_1, 
																	std::placeholders::_2, 
																	std::placeholders::_3));
	}
	
}

void GM_Roguelike::OnPlayerVisitedRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex, 
										const Room& visitRoom, 
										const Craft::Vector2Int& playerPosition)
{
	currentPlayerVisitRoomIndex = visitRoomIndex;
	currentPlayerVisitRoomType = visitRoom.GetRoomType();

	switch (currentPlayerVisitRoomType)
	{
	case eRoomType::Battle:
		{
			OnPlayerVisitedBattleRoom(visitRoom, playerPosition);
		}
		break;

	case eRoomType::Treasure:
		{
			/* TODO : 랜덤한 위치에 임의의 아이템 생성 */
			OutputDebugStringA("Enter Treasure Room\n");
		}
		break;

	case eRoomType::NextLevel:
		{
			OutputDebugStringA("Enter NextLevel Room\n");
		}
		break;
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
	const int spawnMinRange = min(2, maxSpawnNum);
	const int spawnMaxRange = min(4, maxSpawnNum);
	int spawnMonsterNum = Util::RandomRange(spawnMinRange, spawnMaxRange);
	int currentIndex = 0;

	spawnedNPCNum = 0;
	while (spawnMonsterNum > 0 && 
		currentIndex < static_cast<int>(shuffleSpawnIndex.size()))
	{
		const size_t spawnTileIndex = shuffleSpawnIndex[currentIndex++];
		const Vector2Int& spawnTilePos = spawnTileIndices[spawnTileIndex];

		//랜덤으로 결정된 위치에 몬스터 스폰
		std::shared_ptr<NPCBase> spawnedNPC = level->SpawnActor<NPCSlime>(spawnTilePos, currentPlayerVisitRoomIndex);
		assert(spawnedNPC && "Spawn NPC Fail..");

		/* 생성된 NPC가 사망했을때의 이벤트 설정 */
		spawnedNPC->SetDeathEventCallback(std::bind(&GM_Roguelike::OnEventNPCDeath, this, std::placeholders::_1));

		++spawnedNPCNum;
		--spawnMonsterNum;
	}

	/* 방의 입구에 문 Actor 생성 */
	const RoomSpace::RoomTileIndices& doorTileIndices = visitRoomSpace.GetDoorTileIndices();
	for (const Vector2Int& doorTileIndex : doorTileIndices)
	{
		std::shared_ptr<RoomDoor> spawnedRoomDoor = level->SpawnActor<RoomDoor>(doorTileIndex);
		assert(spawnedRoomDoor && "Spawn Door Actor Fail..");

		spawnedRoomDoors.emplace_back(spawnedRoomDoor);
	}
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
}

void GM_Roguelike::OnEventNPCDeath(std::shared_ptr<Pawn> deathPawn)
{
	std::shared_ptr<NPCBase> deathNPC = Cast<NPCBase>(deathPawn);
	assert(deathNPC && "Invalid deathNPC");

	/* 현재 이벤트가 진행중인 방에서 생성된 NPC인 경우에만 NPC 수 감소 */
	if (currentPlayerVisitRoomIndex == deathNPC->GetSpawnedRoomIndex())
	{
		--spawnedNPCNum;
		if (spawnedNPCNum == 0)
		{
			/* 모든 NPC가 죽었으면 현재 방에서의 전투가 끝났다고 간주 */
			OnRoomBattleEnd();
		}
	}
}
