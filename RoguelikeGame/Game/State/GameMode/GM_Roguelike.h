#pragma once

#include "GameState/GameMode/GameMode.h"
#include "Math/Vector2Int.h"
#include "Types/Defines.h"
#include "Types/Enums.h"
#include <vector>
#include <set>

namespace Craft
{
	class Vector2Int;
	class Level;
	class PlayerState;
}

class Room;
class RoomDoor;
class Pawn;
class PlayerStart;
class PlayerPawn;
class NPCBase;

/* 로그라이크 게임 모드(이벤트 및 규칙 전담) */
class GM_Roguelike : public Craft::GameMode
{
	TYPE_DECLARATIONS(GM_Roguelike, GameMode)

public:
	GM_Roguelike();
	virtual ~GM_Roguelike();

public:
	/* 현재 활성화된 레벨이 초기화될 때 호출됨 */
	virtual void OnInitializeLevel(std::weak_ptr<Craft::Level> level) override;

	/* 현재 활성화되어 있던 레벨이 지워지기전 호출됨 */
	virtual void OnDestroyedCurrentLevel() override;

public:
	/* 치트키용 : 다음 층으로 가는 방의 입구 타일 위치 반환 */
	const Craft::Vector2Int& GetNextFloorRoomDoorPosition() const;

private:
	/* 플레이어 상태 객체 생성 */
	virtual std::unique_ptr<Craft::PlayerState> CreatePlayerState() const override;

private:
	/* 구성된 맵 정보를 바탕으로 Game 진행에 필요한 Actor 스폰 및 준비 */
	void ReadyGameActorSpawn();

	/* 플레이어 폰 스폰 */
	void PlayerPawnSpawn();

	/* 레벨 전환하면서 던전 진행 층수 증가*/
	void IncrementFloorLevel();

private:
	/* 플레이어가 맵 내의 특정 방에 진입 했을 때 호출되는 이벤트 콜백 */
	void OnPlayerVisitedRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition);

	/* 플레이어가 맵 내의 특정 방을 벗어났을때 호출되는 이벤트 콜백 */
	void OnPlayerLeavedRoom(const Room& leaveRoom, const Craft::Vector2Int& playerPosition);

	/* 특정 몬스터의 사망 이벤트 수신 */
	void OnEventNPCDeath(std::shared_ptr<Pawn> deathPawn);

	/* 플레이어의 사망 이벤트 수신 */
	void OnEventPlayerDeath(std::shared_ptr<Pawn> deathPawn);

private:
	/* 배틀이 진행되는 방 처리 */
	void OnPlayerVisitedBattleRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition);

	/* 보물창고 방 처리 */
	void OnPlayerVisitedTreasureRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition);

	/* 다음 층으로 향하는 출구 방 처리 */
	void OnPlayerVisitedNextRoom(const Room& visitRoom, const Craft::Vector2Int& playerPosition);

	/* 배틀 종료 처리 */
	void OnRoomBattleEnd();

private:
	/* 현재 맵에서 생성된 플레이어 시작 Actor */
	std::weak_ptr<PlayerStart> spawnedPlayerStart;

	/* 현재 맵에서 생성된 플레이어 폰 */
	std::weak_ptr<PlayerPawn> playerPawn;

	/* 현재 맵에서 플레이어가 방문했던 방 */
	std::set<RoomDefines::UNIQUE_INDEX_TYPE> playerVisitedRoomIndices;

private:
	/* 현재 플레이어가 입장한 방 인덱스 */
	RoomDefines::UNIQUE_INDEX_TYPE currentPlayerVisitRoomIndex = RoomDefines::ROOM_INDEX_INVALID;

	/* 현재 플레이어가 입장한 방의 종류 */
	eRoomType currentPlayerVisitRoomType = eRoomType::None;

	/* BattleRoom에서 진행중일때 방에 소환된 몬스터 수 */
	std::vector<std::shared_ptr<NPCBase>> spawnedNPCList;

	/* BattleRoom에서 진행중일때 방 입구에 배치된 Door */
	std::vector<std::shared_ptr<RoomDoor>> spawnedRoomDoors;

	/* 현재 진행 층수 */
	int currentFloorLevel = 0;
	
	/* 현재 층에서 다음 레벨로 향하는 방의 입구 타일 위치 */
	Craft::Vector2Int nextFloorRoomDoorPos = Craft::Vector2Int::Zero;
};

