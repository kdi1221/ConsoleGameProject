#pragma once

#include "Level/Level.h"
#include "Types/Enums.h"
#include "Types/Defines.h"
#include <Memory>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace Craft
{
	class Actor;
}

class Tilemap;
class Room;
class PlayerStart;
class Pawn;
class PlayerPawn;
class ActorOnTile;

//타일맵 기반 레벨
class TilemapLevel : public Craft::Level
{
	TYPE_DECLARATIONS(TilemapLevel, Level)

public:
	/* 타일위에 Actor가 위치 가능한지 여부결과 값 */
	enum class eCheckPlacementResult
	{
		CanMove,
		InactiveActor,
		SamePosition,
		BlockWall,
		BlockActor,
		Unknown
	};

	/* 벽, Actor등과 Block 되었는지 여부 결과 값 */
	enum class eBlockingCheckResult
	{
		NoBlock,
		BlockWall,
		BlockActor,
	};

	/* 현재 타일맵 레벨 종류 */
	enum class eLevelCategory
	{
		None = 0,

		/* BSP로 생성된 랜덤 미로 */
		MAZE_BSP,

		/* 보스 방 */
		BOSS_ROOM
	};

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
	TilemapLevel(eLevelCategory category);
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
	eCheckPlacementResult CanNextMove(std::shared_ptr<Craft::Actor> checkActor, const Craft::Vector2Int& nextPosition) const;

	/* checkActor와 충돌되는 대상을 장애물로 인식하고 currentPosition에서 nextPosition으로 움직일 수 있는지 확인 */
	eCheckPlacementResult CanNextMove(std::shared_ptr<Craft::Actor> checkActor, const Craft::Vector2Int& currentPosition, const Craft::Vector2Int& nextPosition) const;

	/* direction이 대각 방향인 경우 x, y 각 방향에 장애물이 있는지 체크 */
	bool IsDiagonalBlocked(std::shared_ptr<Craft::Actor> checkActor, const Craft::Vector2Int& currentPosition, const Craft::Vector2Int& direction) const;

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
	/* 현재 레벨 종류 반환 */
	eLevelCategory GetLevelCategory() const;

	/* 해당 위치에 Block되는 Actor가 존재하는지 여부 반환 */
	eBlockingCheckResult CheckBlocking(std::shared_ptr<Craft::Actor> checkActor, 
										const Craft::Vector2Int& tileCoord,
										std::shared_ptr<ActorOnTile>& outBlockingActor) const;

	/* 해당 위치에 존재하는 Pawn찾아서 반환 */
	std::shared_ptr<Pawn> GetPawnOnTile(const Craft::Vector2Int& tileCoord) const;

	/* 해당 위치가 속한 방 반환 */
	const Room* GetPostionInRoom(const Craft::Vector2Int& checkPosition) const;

	/* 해당 위치의 타일 종류 반환 */
	eTileCategory GetTileCategory(const Craft::Vector2Int& position) const;

	/* 해당 위치의 타일이 속한 방 인덱스 반환 */
	RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndexInTile(const Craft::Vector2Int& position) const;

	/* Actor를 중심으로 주변 내 빈 Ground Tile 반환(BFS) */
	void GetAvailableTilesInRange(std::shared_ptr<Craft::Actor> checkActor,
								const int checkRange,
								std::vector<Craft::Vector2Int>& outerPoints,
								std::vector<Craft::Vector2Int>& innerPoints,
								const float aspectRatio = 1.3f) const;

private:
	/* BSP를 활용한 랜덤타일맵 생성 */
	void BuildTilemapBSP();

	/* 각 방의 종류 지정(BSP) */
	void AssignRoomType();

	/* 보스방 고정 타일맵 생성 */
	void BuildTilemapBossRoom();

	/* Actor가 점유하는 타일 등록 */
	void RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);

	/* Actor가 점유하는 타일 등록 해제 */
	void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);
	void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Craft::Vector2Int& centerPosition);


	/* 타일맵 인덱스별로 Actor 등록 */
	//void RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);
	//void RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Craft::Vector2Int& position);

	/* 타일맵 인덱스별로 Actor 등록 해제 */
	//void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile);
	//void UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Craft::Vector2Int& position);




	/* 플레이어 캐릭터의 이동에 따른 이벤트 구성 */
	void OnMovePlayerEvent(const Craft::Vector2Int& prevWorldPosition, const Craft::Vector2Int& worldPosition);

private:
	/* 현재 레벨 종류(일반 필드 / 보스방) */
	eLevelCategory levelCategory = eLevelCategory::None;

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

using CheckPlacementResult = TilemapLevel::eCheckPlacementResult;
using CheckBlockingResult = TilemapLevel::eBlockingCheckResult;