#include "AbilitySummon.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include "Level/TilemapLevel.h"
#include "Tilemap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Util/Util.h"
#include "Game/State/GameMode/GM_Roguelike.h"
#include <Engine/Engine.h>
#include <Math/Vector2Int.h>
#include <cassert>

using namespace Craft;

AbilitySummon::AbilitySummon(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
}

void AbilitySummon::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (IsActivate())
	{
		CheckSummonDelayAndExecute(deltaTime);
	}
}

void AbilitySummon::ActivateAbility()
{
	super::ActivateAbility();
}

void AbilitySummon::EndAbility(bool bCancelAbility)
{
	super::EndAbility(bCancelAbility);

	//Ability가 종료되면 소환되었던 몬스터들 제거
	DestorySummonMonster();
}

void AbilitySummon::SetSummonDelay(float delayTime)
{
	summonDelayTimer.SetTargetTime(delayTime);
}

void AbilitySummon::SetSummonNPCChaseTarget(std::weak_ptr<Pawn> target)
{
	chaseTarget = target;
}

void AbilitySummon::SetMaxSummonNum(int maxNum)
{
	maxSummonNum = maxNum;
}

void AbilitySummon::SetMaxSummonDelayNum(int maxNum)
{
	maxSummonDelayNum = maxNum;
}

void AbilitySummon::CheckSummonDelayAndExecute(float deltaTime)
{
	summonDelayTimer.Tick(deltaTime);
	if (summonDelayTimer.IsTimeOut())
	{
		ExecuteSummon();
		summonDelayTimer.Reset();
	}
}

void AbilitySummon::ExecuteSummon()
{
	//현재 몬스터 마리수 체크(최대 마리수가 넘어서면 소환하지 못함)
	const int allowSummonNum = maxSummonNum - static_cast<int>(summonNPCs.size());
	if (allowSummonNum <= 0)
	{
		return;
	}


	GM_Roguelike* gameMode = Engine::Get().GetGameMode<GM_Roguelike>();
	assert(gameMode && "Invalid gameMode");

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerPawn");

	std::shared_ptr<TilemapLevel> currentLevel = Cast<TilemapLevel>(ownerPawn->GetOwner());
	assert(currentLevel && "Invalid currentLevel");

	const Room* locatedRoom = currentLevel->GetPostionInRoom(ownerPawn->GetPosition());
	assert(locatedRoom && "Invalid locatedRoom");

	/* 현재 위치한 방의 인덱스 */
 	RoomDefines::UNIQUE_INDEX_TYPE locatedRoomIndex = locatedRoom->GetRoomUniqueIndex();

	/* 현재 위치한 방의 영역 정보 조회 */
	const RoomSpace& locatedRoomSpace = locatedRoom->GetRoomSpace();

	/* 셔플 알고리즘 - 소환 가능한 위치들을 뒤섞음*/
	std::vector<size_t> shuffleSpawnIndex;
	const RoomSpace::RoomTileIndices spawnTileIndices = locatedRoomSpace.GetInnerTileIndices();
	for (size_t tileIndex = 0; tileIndex < spawnTileIndices.size(); ++tileIndex)
	{
		/* 다른 Pawn이 점유중인 타일은 제외해야한다. */
		if (currentLevel->IsTileCoordinateOccupiedPawn(spawnTileIndices[tileIndex]))
		{
			continue;
		}

		shuffleSpawnIndex.emplace_back(tileIndex);
	}

	/* 방 안에 몬스터가 위치할 타일들이 존재하지 않으면 반환 */
	const int maxAllowTileNum = static_cast<int>(shuffleSpawnIndex.size());
	if (maxAllowTileNum <= 0)
	{
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
	int spawnMonsterNum = min(min(maxSummonDelayNum, maxAllowTileNum), allowSummonNum);

	/* 몬스터 스폰 */
	int currentShuffleTileIndex = 0;
	while (spawnMonsterNum > 0 &&
		currentShuffleTileIndex < static_cast<int>(shuffleSpawnIndex.size()))
	{
		const size_t spawnTileIndex = shuffleSpawnIndex[currentShuffleTileIndex++];
		const Vector2Int& spawnTilePos = spawnTileIndices[spawnTileIndex];

		//GameMode를 통해 NPC 스폰
		std::shared_ptr<NPCBase> spawnedNPC = gameMode->SpawnRandomNPC(spawnTilePos, locatedRoomIndex, 3);
		assert(spawnedNPC && "Spawn NPC Fail");

		/* 생성된 몬스터가 사망했을때의 이벤트 설정 */
		spawnedNPC->SetDeathEventCallback(std::bind(&AbilitySummon::OnEventSummonNPCDeath, this, std::placeholders::_1));

		/* 소환 NPC 관리 리스트에 추가 */
		summonNPCs.insert({ spawnedNPC->GetUniqueID(), spawnedNPC });

		--spawnMonsterNum;

		/* 소환된 몬스터들에게 플레이어 추적 명령 내림 */
		spawnedNPC->SetChaseTarget(chaseTarget);
	}
}

void AbilitySummon::DestorySummonMonster()
{
	for (auto& summonNPC : summonNPCs)
	{
		std::shared_ptr<NPCBase> summonNPCPtr = summonNPC.second.lock();
		if (!summonNPCPtr)
		{
			continue;
		}

		summonNPCPtr->Destroy();
	}

	summonNPCs.clear();
}

void AbilitySummon::OnEventSummonNPCDeath(std::shared_ptr<Pawn> deathPawn)
{
	if (!deathPawn)
	{
		return;
	}

	summonNPCs.erase(deathPawn->GetUniqueID());
}
