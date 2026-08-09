#pragma once

#include "Level/Level.h"
#include "Types/Defines.h"
#include <Memory>
#include <map>

namespace Craft
{
	class Actor;
	class Vector2Float;
}

class Tilemap;
class Room;
class PlayerStart;
class PlayerPawn;

//타일맵 기반 레벨
class TilemapLevel : public Craft::Level
{
public:
	using RoomMapType = std::map<RoomDefines::UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>;

public:
	TilemapLevel();
	virtual ~TilemapLevel();

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	/* 다음 위치(nextPosition)로 actor가 이동할 수 있는지 확인 */
	virtual bool CanNextMove(const Craft::Actor& checkActor, const Craft::Vector2Float& nextPosition) override;

private:
	/* BSP를 활용한 랜덤타일맵 생성 */
	void BuildTilemapBSP();

	/* 각 방의 종류 지정 */
	void AssignRoomType();

	/* 구성된 맵 정보를 바탕으로 Game 진행에 필요한 Actor 스폰 및 준비*/
	void ReadyGame();

	/* 플레이어 폰 스폰 */
	void PlayerPawnSpawn();

private:
	/* 맵 상의 존재하는 타일맵 객체 */
	std::unique_ptr<Tilemap> tileMap;

	//맵 상의 방 정보들
	RoomMapType mapRooms;

	//생성된 플레이어 스타트 액터
	std::shared_ptr<PlayerStart> spawnedPlayerStart;

	//생성된 플레이어 폰
	std::shared_ptr<PlayerPawn> playerPawn;
};

