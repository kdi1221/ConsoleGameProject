#pragma once

#include "Level/Level.h"
#include "Types/Defines.h"
#include <Memory>
#include <map>
#include <unordered_map>
#include <functional>

namespace Craft
{
	class Actor;
}

class Tilemap;
class Room;
class PlayerStart;
class PlayerPawn;
class ActorOnTile;

//타일맵 기반 레벨
class TilemapLevel : public Craft::Level
{
	TYPE_DECLARATIONS(TilemapLevel, Level)

public:
	using RoomMapType = std::map<RoomDefines::UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>;
	using TilemapActorListType = std::unordered_map<Craft::Vector2Int, std::vector<std::weak_ptr<ActorOnTile>>>;
	using PlayerVistedRoomEventType = std::function<void(const RoomDefines::UNIQUE_INDEX_TYPE, const Room&, const Craft::Vector2Int&)>;

public:
	TilemapLevel();
	virtual ~TilemapLevel();

protected:
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

protected:
	/* 특정 Actor가 레벨안에 스폰됨 */
	virtual void OnSpawnedActor(std::shared_ptr<Craft::Actor> spawnedActor);

	/* 레벨안의 특정 Actor의 위치 업데이트 */
	virtual void OnPositionUpdateActorInLevel(std::weak_ptr<Craft::Actor> updatedActor,
											const Craft::Vector2Int& prevWorldPosition,
											const Craft::Vector2Int& worldPosition);

	/* 레벨안의 특정 Actor Destroy */
	virtual void OnDestroyedActorInLevel(std::weak_ptr<Craft::Actor> destoryedActor);

protected:
	/* 다음 위치(nextPosition)로 actor가 이동할 수 있는지 확인 */
	virtual bool CanNextMove(std::shared_ptr<Craft::Actor> checkActor, const Craft::Vector2Int& nextPosition) override;

public:
	/* 타일맵 기반 충돌 처리 */
	void ProcessTilemapCollision();

	/* 플레이어의 특정 방 진입 이벤트에 대한 콜백 등록 */
	void SetPlayerVisitedRoomEventCallback(PlayerVistedRoomEventType inCallback);

private:
	/* BSP를 활용한 랜덤타일맵 생성 */
	void BuildTilemapBSP();

	/* 각 방의 종류 지정 */
	void AssignRoomType();

	/* 구성된 맵 정보를 바탕으로 Game 진행에 필요한 Actor 스폰 및 준비*/
	void ReadyGame();

	/* 플레이어 폰 스폰 */
	void PlayerPawnSpawn();

	/* 타일맵 인덱스별로 Actor 등록 */
	void RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);
	void RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Craft::Vector2Int& position);

	/* 타일맵 인덱스별로 Actor 등록 해제 */
	void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);
	void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Craft::Vector2Int& position);

	/* 플레이어 캐릭터의 이동에 따른 이벤트 구성 */
	void OnMovePlayerEvent(const Craft::Vector2Int& prevWorldPosition, const Craft::Vector2Int& worldPosition);

private:
	/* 맵 상의 존재하는 타일맵 객체 */
	std::unique_ptr<Tilemap> tileMap;

	//맵 상의 방 정보들
	RoomMapType mapRooms;

	//생성된 플레이어 스타트 액터
	std::shared_ptr<PlayerStart> spawnedPlayerStart;

	//생성된 플레이어 폰
	std::shared_ptr<PlayerPawn> playerPawn;

	//타일맵 내 타일 인덱스마다 존재하는 Actor 리스트들
	TilemapActorListType mapActorListOnTilemap;

	/* 플레이어가 특정 방을 최초로 방문했을때 호출되는 이벤트 */
	PlayerVistedRoomEventType onPlayerVisitedRoom;
};

