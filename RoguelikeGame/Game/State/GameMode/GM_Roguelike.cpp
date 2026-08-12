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

void GM_Roguelike::OnPlayerVisitedRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex, 
										const Room& visitRoom, 
										const Craft::Vector2Int& playerPosition)
{
	switch (visitRoom.GetRoomType())
	{
	case eRoomType::Battle:
		{
			OnPlayerVisitedBattleRoom(visitRoomIndex, visitRoom, playerPosition);
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

void GM_Roguelike::OnPlayerVisitedBattleRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex, 
											const Room& visitRoom, 
											const Craft::Vector2Int& playerPosition)
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

	while (spawnMonsterNum > 0 && 
		currentIndex < static_cast<int>(shuffleSpawnIndex.size()))
	{
		const size_t spawnTileIndex = shuffleSpawnIndex[currentIndex++];
		const Vector2Int& spawnTilePos = spawnTileIndices[spawnTileIndex];

		//랜덤으로 결정된 위치에 몬스터 스폰
		level->SpawnActor<NPCSlime>(spawnTilePos);

		--spawnMonsterNum;
	}

	/* 방의 입구에 문 Actor 생성 */
	const RoomSpace::RoomTileIndices& doorTileIndices = visitRoomSpace.GetDoorTileIndices();
	for (const Vector2Int& doorTileIndex : doorTileIndices)
	{
		level->SpawnActor<RoomDoor>(doorTileIndex);
	}
}
