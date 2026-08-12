#pragma once

#include "GameState/GameMode/GameMode.h"
#include "Types/Defines.h"

namespace Craft
{
	class Vector2Int;
}

class Room;

/* 로그라이크 게임 모드(이벤트 및 규칙 전담) */
class GM_Roguelike : public Craft::GameMode
{
	TYPE_DECLARATIONS(GM_Roguelike, GameMode)

public:
	GM_Roguelike();
	virtual ~GM_Roguelike();

public:
	/* 플레이어가 맵 내의 특정 방에 진입 했을 때 호출 */
	void OnPlayerVisitedRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex,
							const Room& visitRoom,
							const Craft::Vector2Int& playerPosition);

private:
	void OnPlayerVisitedBattleRoom(const RoomDefines::UNIQUE_INDEX_TYPE visitRoomIndex,
									const Room& visitRoom,
									const Craft::Vector2Int& playerPosition);
};

