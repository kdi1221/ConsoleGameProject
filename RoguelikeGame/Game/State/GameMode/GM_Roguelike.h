#pragma once

#include "GameState/GameMode/GameMode.h"
#include "Types/Defines.h"
#include "Types/Enums.h"
#include <vector>

namespace Craft
{
	class Vector2Int;
	class Level;
}

class Room;
class RoomDoor;
class Pawn;

/* 로그라이크 게임 모드(이벤트 및 규칙 전담) */
class GM_Roguelike : public Craft::GameMode
{
	TYPE_DECLARATIONS(GM_Roguelike, GameMode)

public:
	GM_Roguelike();
	virtual ~GM_Roguelike();

public:
	/* 레벨이 설정될때 호출 */
	virtual void SetCurrentLevel(std::weak_ptr<Craft::Level> level) override;

private:
	/* 플레이어가 맵 내의 특정 방에 진입 했을 때 호출되는 이벤트 콜백 */
	void OnPlayerVisitedRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex,
		const Room& visitRoom,
		const Craft::Vector2Int& playerPosition);

	/* 특정 몬스터의 사망 이벤트 수신 */
	void OnEventNPCDeath(std::shared_ptr<Pawn> deathPawn);

private:
	/* 배틀이 진행되는 방 처리 */
	void OnPlayerVisitedBattleRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition);

	/* 배틀 종료 처리 */
	void OnRoomBattleEnd();

private:
	/* 현재 플레이어가 입장한 방 인덱스 */
	RoomDefines::UNIQUE_INDEX_TYPE currentPlayerVisitRoomIndex = RoomDefines::ROOM_INDEX_INVALID;

	/* 현재 플레이어가 입장한 방의 종류 */
	eRoomType currentPlayerVisitRoomType = eRoomType::None;

	/* BattleRoom에서 진행중일때 방에 소환된 몬스터 수 */
	unsigned int spawnedNPCNum = 0;

	/* BattleRoom에서 진행중일때 방 입구에 배치된 Door */
	std::vector<std::shared_ptr<RoomDoor>> spawnedRoomDoors;
	
};

