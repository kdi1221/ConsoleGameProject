#pragma once

#include "Level/Level.h"
#include "Types/Enums.h"
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

private:
	struct FTilemapDamageInfo
	{
		float damageValue = 0.f;
		eTeamID teamID = eTeamID::None;

		FTilemapDamageInfo()
		{

		}

		FTilemapDamageInfo(float damage, eTeamID team)
			:damageValue(damage)
			,teamID(team)
		{

		}
	};

public:
	using RoomMapType = std::map<RoomDefines::UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>;
	using TilemapActorListType = std::unordered_map<Craft::Vector2Int, std::vector<std::weak_ptr<ActorOnTile>>>;
	using PlayerRoomEventType = std::function<void(const Room&, const Craft::Vector2Int&)>;
	using TilemapDamageListType = std::unordered_map<Craft::Vector2Int, std::vector<FTilemapDamageInfo>>;

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

public:
	/* 다음 위치(nextPosition)로 actor가 이동할 수 있는지 확인 */
	virtual bool CanNextMove(std::shared_ptr<Craft::Actor> checkActor, const Craft::Vector2Int& nextPosition) override;

public:
	/* 타일맵 기반 충돌 처리 */
	void ProcessTilemapCollision();

	/* 플레이어의 특정 방 진입 이벤트에 대한 콜백 등록 */
	void SetPlayerVisitedRoomEventCallback(PlayerRoomEventType inCallback);

	/* 플레이어의 특정 방 철수 이벤트에 대한 콜백 등록 */
	void SetPlayerLeavedRoomEventCallback(PlayerRoomEventType inCallback);

	/* 현재 생성된 방들 정보 조회 */
	void Foreach_Rooms(std::function<void(const Room&)> callback);

	/* 지정된 타일에 데미지 처리 추가 */
	void AddDamageInfoToTile(const Craft::Vector2Int& tileCoord, float damageValue, eTeamID teamID);

public:
	/* 해당 위치가 속한 방 반환 */
	const Room* GetPostionInRoom(const Craft::Vector2Int& checkPosition) const;

	/* 해당 위치의 타일 종류 반환 */
	eTileCategory GetTileCategory(const Craft::Vector2Int& position) const;

	/* 해당 위치의 타일이 속한 방 인덱스 반환 */
	RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndexInTile(const Craft::Vector2Int& position) const;

private:
	/* BSP를 활용한 랜덤타일맵 생성 */
	void BuildTilemapBSP();

	/* 각 방의 종류 지정 */
	void AssignRoomType();

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

	//타일맵 내 타일 인덱스마다 존재하는 Actor 리스트들
	TilemapActorListType mapActorListOnTilemap;

	/* 플레이어가 특정 방을 방문했을때 호출되는 이벤트 */
	PlayerRoomEventType onPlayerVisitedRoom;

	/* 플레이어가 특정 방을 떠났을 때 호출되는 이벤트 */
	PlayerRoomEventType onPlayerLeavedRoom;

	//타일맵 내 타일 인덱스마다 체크할 데미지 정보(Ability)
	TilemapDamageListType mapDamageListOnTilemap;
};

