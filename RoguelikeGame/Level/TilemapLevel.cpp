#include "TilemapLevel.h"
#include "Engine/Engine.h"
#include "Game/Config/Config.h"
#include "Render/Renderer.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/Room/Room.h"
#include "Tilemap/BSP/RoomSpace/RoomSpace.h"
#include "Util/Util.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Actor/Bullet/Bullet.h"
#include "Camera/CameraManager.h"
#include <cassert>
#include <Memory>

using namespace Craft;
using namespace RoomDefines;

TilemapLevel::TilemapLevel()
{

}

TilemapLevel::~TilemapLevel()
{

}

void TilemapLevel::OnInitialized()
{
	Level::OnInitialized();

	/* BSP로 타일맵 구성 */
	BuildTilemapBSP();

	/* 만들어진 방들의 종류 지정 */
	AssignRoomType();
}

void TilemapLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	if (tileMap)
	{
		tileMap->Tick(deltaTime);
	}
}

void TilemapLevel::Draw()
{
	if (tileMap)
	{
		tileMap->Draw();
	}

	Level::Draw();
}

void TilemapLevel::OnSpawnedActor(std::shared_ptr<Actor> spawnedActor)
{
	super::OnSpawnedActor(spawnedActor);

	/* 새로 생성된 Actor가 Tile위에 올라갈 수 있는 Actor일 때만 등록 */
	std::shared_ptr<ActorOnTile> actorOnTile = Cast<ActorOnTile>(spawnedActor);
	if (!actorOnTile)
	{
		return;
	}

	RegisterActorOnTilemap(actorOnTile);
}

void TilemapLevel::OnPositionUpdateActorInLevel(std::weak_ptr<Actor> updatedActor, 
												const Vector2Int& prevWorldPosition, 
												const Vector2Int& worldPosition)
{
	super::OnPositionUpdateActorInLevel(updatedActor, prevWorldPosition, worldPosition);

	std::shared_ptr<ActorOnTile> actorOnTile = Cast<ActorOnTile>(updatedActor.lock());
	if (!actorOnTile)
	{
		return;
	}

	/* 이전 위치에서 등록 해제*/
	UnregisterActorOnTilemap(actorOnTile, prevWorldPosition);

	/* 새로운 위치에 등록 */
	RegisterActorOnTilemap(actorOnTile);

	/* 플레이어의 이동 이벤트 처리 */
	if (actorOnTile->IsTypeOf<PlayerPawn>())
	{
		OnMovePlayerEvent(prevWorldPosition, worldPosition);
	}
}

void TilemapLevel::OnDestroyedActorInLevel(std::weak_ptr<Actor> destoryedActor)
{
	super::OnDestroyedActorInLevel(destoryedActor);

	std::shared_ptr<ActorOnTile> actorOnTile = Cast<ActorOnTile>(destoryedActor.lock());
	if (!actorOnTile)
	{
		return;
	}

	/* Destroy전 등록 해제 */
	UnregisterActorOnTilemap(actorOnTile);
}

CheckPlacementResult TilemapLevel::CanNextMove(std::shared_ptr<Actor> checkActor, const Vector2Int& nextPosition) const
{
	if (!checkActor || !checkActor->IsActive())
	{
		return CheckPlacementResult::InactiveActor;
	}

	/* 같은 위치라면 이동 불가 */
	if (checkActor->GetWorldPosition() == nextPosition)
	{
		return CheckPlacementResult::SamePosition;
	}

	/* 다음에 이동할 위치가 벽타일이면 이동 불가 */
	const eTileCategory nextTileCategory = tileMap->GetTileCategory(nextPosition);
	if (eTileCategory::Wall == nextTileCategory)
	{
		return CheckPlacementResult::BlockWall;
	}

	/* 다음에 이동할 위치에 Block되는 Actor가 존재하는지 확인 */
	std::shared_ptr<ActorOnTile> checkActorOnTile = Cast<ActorOnTile>(checkActor);
	if (checkActorOnTile)
	{
		const auto& findActorListOnTile = mapActorListOnTilemap.find(nextPosition);

		/* 해당 타일에 존재하는 Actor가 없는 경우 이동 가능 */
		if (findActorListOnTile == mapActorListOnTilemap.end())
		{
			return CheckPlacementResult::CanMove;
		}

		auto& actorListOnTile = findActorListOnTile->second;
		auto find_blockActor = [&checkActorOnTile](std::weak_ptr<ActorOnTile> actorOnTile)
			{
				std::shared_ptr<ActorOnTile> checkBlockActor = actorOnTile.lock();

				// 액터가 유효하지 않거나 비활성화 상태라면 건너뛰기
				if (!checkBlockActor || !checkBlockActor->IsActive())
				{
					return false;
				}

				return checkBlockActor->IsBlockActor(checkActorOnTile);
			};

		/* 해당 위치 타일에 block되는 Actor가 존재하지 않으면 이동 가능 */
		auto iterFindBlockActor = std::find_if(actorListOnTile.begin(), actorListOnTile.end(), find_blockActor);
		return iterFindBlockActor == actorListOnTile.end() ? CheckPlacementResult::CanMove : CheckPlacementResult::BlockActor;
	}

	/* 그 외의 경우 모두 이동 가능*/
	return CheckPlacementResult::CanMove;
}

void TilemapLevel::ProcessTilemapCollision()
{
	assert(tileMap && "Invalid tileMap");

	// Actor가 위치한 타일의 종류에 따른 처리
	using TileOverlapActorPair = std::pair<eTileCategory, std::shared_ptr<ActorOnTile>>;
	std::vector<TileOverlapActorPair> tileOverlapActorList;
	for (auto& iterActorListOnTile : mapActorListOnTilemap)
	{
		const Vector2Int& tilePosition = iterActorListOnTile.first;
		auto& actorListOnTile = iterActorListOnTile.second;

		const eTileCategory tileCategory = tileMap->GetTileCategory(tilePosition);
		for (std::weak_ptr<ActorOnTile>& ptrActorOnTile : actorListOnTile)
		{
			std::shared_ptr<ActorOnTile> actorOnTile = ptrActorOnTile.lock();

			// 액터가 유효하지 않거나 비활성화 상태라면 건너뛰기
			if (!actorOnTile || !actorOnTile->IsActive())
			{
				continue;
			}

			tileOverlapActorList.emplace_back(TileOverlapActorPair(tileCategory, actorOnTile));
		}
	}

	//수집된 타일별 Actor들에 대해 일괄적으로 TileOverlap 알림을 보낸다.
	for (const TileOverlapActorPair& pair : tileOverlapActorList)
	{
		eTileCategory overlapTileCategory = pair.first;
		const std::shared_ptr<ActorOnTile>& overlapActor = pair.second;

		//앞선 충돌 체크등에서 이미 삭제되거나 비활성화된 Actor 제외
		if (!overlapActor->IsActive())
		{
			continue;
		}

		overlapActor->OnTileOverlap(overlapTileCategory);
	}

	/* 타일 내 겹치는 Actor들의 충돌 쌍들 검출 */
	using CollisionActorPair = std::pair<std::shared_ptr<ActorOnTile>, std::shared_ptr<ActorOnTile>>;
	auto makeCollidedSet = [](std::vector<std::weak_ptr<ActorOnTile>>& inActorListOnTile, std::vector<CollisionActorPair>& outCollidedActorList)
		{
			const size_t actorListNum = inActorListOnTile.size();

			/* 타일 내 액터들을 순회하면서 충돌 쌍들 검출 */
			for (size_t ix = 0; ix < actorListNum; ++ix)
			{
				std::shared_ptr<ActorOnTile> leftActor = inActorListOnTile[ix].lock();

				// 액터가 유효하지 않거나 비활성화 상태라면 건너뛰기
				if (!leftActor || !leftActor->IsActive())
				{
					continue;
				}

				for (size_t jx = ix + 1; jx < actorListNum; ++jx)
				{
					std::shared_ptr<ActorOnTile> rightActor = inActorListOnTile[jx].lock();

					// 액터가 유효하지 않거나 비활성화 상태라면 건너뛰기
					if (!rightActor || !rightActor->IsActive())
					{
						continue;
					}

					//같은 타일에 겹쳐있으면 충돌되었다고 판정하고 충돌 목록에 추가함
					outCollidedActorList.emplace_back(CollisionActorPair(leftActor, rightActor));
				}
			}
		};

	//충돌 처리에 대한 알림을 보낼 Actor 쌍들 생성
	std::vector<CollisionActorPair> collidedActorList;

	//타일맵 기반 충돌 처리 : 한 타일에 겹쳐있는 Actor들끼리만 처리한다.
	for (auto& iterActorListOnTile : mapActorListOnTilemap)
	{	
		makeCollidedSet(iterActorListOnTile.second, collidedActorList);

		if (collidedActorList.empty())
		{
			continue;
		}
	}

	//충돌한 Actor 쌍들에 각각 이벤트 전달
	for (const CollisionActorPair& pair : collidedActorList)
	{
		//앞선 충돌 체크등에서 이미 삭제되거나 비활성화된 Actor 제외
		const std::shared_ptr<ActorOnTile>& leftActor = pair.first;
		const std::shared_ptr<ActorOnTile>& rightActor = pair.second;
		if (!leftActor->IsActive() || !rightActor->IsActive())
		{
			continue;
		}

		leftActor->OnCollision(rightActor);
		rightActor->OnCollision(leftActor);
	}

	//타일별 데미지 리스트 처리
	for (auto& iterDamageListOnTile : mapDamageListOnTilemap)
	{
		const Vector2Int& tilePosition = iterDamageListOnTile.first;
		
		auto findTileOnActorList = mapActorListOnTilemap.find(tilePosition);
		if (findTileOnActorList == mapActorListOnTilemap.end())
		{
			continue;
		}

		auto& damageListOnTile = iterDamageListOnTile.second;
		auto& actorListOnTile = findTileOnActorList->second;

		for (std::weak_ptr<ActorOnTile>& ptrActorOnTile : actorListOnTile)
		{
			std::shared_ptr<Pawn> actorOnPawn = Cast<Pawn>(ptrActorOnTile.lock());

			// 액터가 유효하지 않거나(Pawn이 아니거나) 비활성화 상태라면 건너뛰기
			if (!actorOnPawn || !actorOnPawn->IsActive())
			{
				continue;
			}

			for (const FTilemapDamageInfo& damageInfo : damageListOnTile)
			{
				if (actorOnPawn->GetTeamID() != damageInfo.teamID)
				{
					actorOnPawn->TakeDamage(damageInfo.damageValue);
				}
			}
		}
	}
	mapDamageListOnTilemap.clear();
}

void TilemapLevel::SetPlayerVisitedRoomEventCallback(PlayerRoomEventType inCallback)
{
	onPlayerVisitedRoom = inCallback;
}

void TilemapLevel::SetPlayerLeavedRoomEventCallback(PlayerRoomEventType inCallback)
{
	onPlayerLeavedRoom = inCallback;
}

void TilemapLevel::Foreach_Rooms(std::function<void(const Room&)> callback)
{
	for (const auto& pairRoom : mapRooms)
	{
		if (!pairRoom.second)
		{
			continue;
		}

		callback(*pairRoom.second);
	}
}

void TilemapLevel::AddDamageInfoToTile(const Vector2Int& tileCoord, float damageValue, eTeamID teamID)
{
	auto& damageListOnTile = mapDamageListOnTilemap[tileCoord];
	damageListOnTile.emplace_back(FTilemapDamageInfo(damageValue, teamID));
}

CheckBlockingResult TilemapLevel::CheckBlocking(std::shared_ptr<Craft::Actor> checkActor, 
												const Craft::Vector2Int& tileCoord,
												std::shared_ptr<ActorOnTile>& outBlockingActor) const
{
	/* 1. 해당위치의 벽에 충돌되었는지 확인 */
	if (eTileCategory::Wall == GetTileCategory(tileCoord))
	{
		return CheckBlockingResult::BlockWall;
	}

	/* 2. 해당 위치의 다른 Actor와 Block되었는지 확인 */
	const auto findIterActorListOnTile = mapActorListOnTilemap.find(tileCoord);
	if (findIterActorListOnTile != mapActorListOnTilemap.end())
	{
		const std::vector<std::weak_ptr<ActorOnTile>>& actorListOnTile = findIterActorListOnTile->second;
		for (const std::weak_ptr<ActorOnTile>& actorOnTile : actorListOnTile)
		{
			std::shared_ptr<ActorOnTile> checkAnotherActor = actorOnTile.lock();
			if (!checkAnotherActor)
			{
				continue;
			}

			if (checkAnotherActor->IsBlockActor(checkActor))
			{
				/* 충돌된 액터 정보 반환 */
				outBlockingActor = checkAnotherActor;

				return CheckBlockingResult::BlockActor;
			}
		}
	}

	return CheckBlockingResult::NoBlock;
}

const Room* TilemapLevel::GetPostionInRoom(const Craft::Vector2Int& checkPosition) const
{
	if (!tileMap)
	{
		return nullptr;
	}

	tileMap->GetTileRoomIndex(checkPosition);

	const UNIQUE_INDEX_TYPE findRoomIndex = tileMap->GetTileRoomIndex(checkPosition);
	auto iterStartRoom = mapRooms.find(findRoomIndex);
	if (iterStartRoom == mapRooms.end())
	{
		return nullptr;
	}

	return iterStartRoom->second.get();
}

eTileCategory TilemapLevel::GetTileCategory(const Craft::Vector2Int& position) const
{
	if (!tileMap)
	{
		return eTileCategory::None;
	}

	return tileMap->GetTileCategory(position);
}

RoomDefines::UNIQUE_INDEX_TYPE TilemapLevel::GetRoomIndexInTile(const Craft::Vector2Int& position) const
{
	if (!tileMap)
	{
		return RoomDefines::ROOM_INDEX_INVALID;
	}

	return tileMap->GetTileRoomIndex(position);
}

void TilemapLevel::BuildTilemapBSP()
{
	//이전 타일맵 정보 초기화
	tileMap.reset();

	//새로운 타일맵 생성
	tileMap = std::make_unique<Tilemap>(*this);
	assert(tileMap && "Fail Tilemap Alloc..");

	auto lambdaOpenPath = [this](const std::vector<Craft::Vector2Int>& pathTileIndices)
		{
			for (const Vector2Int& pathTileIndex : pathTileIndices)
			{
				//경로상의 타일 뚫기
				tileMap->SetTileCategory(pathTileIndex.x, pathTileIndex.y, eTileCategory::Ground);
			}
		};

	auto lambdaGenerateRoom = [this](std::unique_ptr<RoomSpace> roomSpace)
		{
			//방 객체 생성 및 타일 구성
			UNIQUE_INDEX_TYPE newRoomIndex = static_cast<UNIQUE_INDEX_TYPE>(mapRooms.size() + 1);

			std::unique_ptr<Room> newRoom = std::make_unique<Room>(newRoomIndex, std::move(roomSpace));
			assert(newRoom && "Invalid newRoom");

			//방 안의 타일들 생성
			const RoomSpace::RoomTileIndices& newRoomInnerTiles = newRoom->GetRoomSpace().GetInnerTileIndices();
			for (const Vector2Int& tileIndex : newRoomInnerTiles)
			{
				tileMap->SetTileRoomIndex(tileIndex.x, tileIndex.y, newRoomIndex);
				tileMap->SetTileCategory(tileIndex.x, tileIndex.y, eTileCategory::Ground);
			}

			mapRooms.insert(std::pair<UNIQUE_INDEX_TYPE, std::unique_ptr<Room>>(newRoomIndex, std::move(newRoom)));
			newRoom.reset();
		};

	const Config& config = Engine::Get().GetConfig<Config>();
	const Vector2Int tilemapSize(config.GetTilemapWidth(), config.GetTilemapHeight());
	tileMap->InitializeTilemap(tilemapSize, lambdaOpenPath, lambdaGenerateRoom);

	/* 카메라 제한 설정, 타일맵 외곽을 벗어나지 못하게 한다. */
	const Vector2Int& tileMapLeftTopPos = tileMap->GetLeftTopPos();
	const Vector2Int& tileMapInnerRect = tileMap->GetInnerTileRect();

	CameraManager& cameraManager = Engine::Get().GetCameraManager();
	cameraManager.SetLimitPosition(tileMapLeftTopPos, tileMapLeftTopPos + tileMapInnerRect - Vector2Int::One);
}

void TilemapLevel::AssignRoomType()
{
	//입구, 출구 등 방 종류들을 랜덤하게 선택한다.
	std::vector<RoomDefines::UNIQUE_INDEX_TYPE> roomKeys;
	for (const auto& pairRoom : mapRooms)
	{
		roomKeys.push_back(pairRoom.first);
	}

	//셔플 알고리즘으로 방의 키값들을 뒤섞는다.
	assert(!roomKeys.empty() && "roomKeys empty");
	for (size_t i = roomKeys.size() - 1; i > 0; --i)
	{
		//0 ~ i 까지 인덱스 중 랜덤하게 선택
		const int j = Util::RandomRange(0, static_cast<int>(i));

		//현재 방 인덱스와 랜덤하게 결정된 방 인덱스를 교환해서 뒤섞는다.
		std::swap(roomKeys[i], roomKeys[j]);
	}

	//앞의 2개는 각각 방의 시작지점과 다음레벨로 지정한다.
	//TODO : 마지막 층이면 다음레벨대신 출구를 지정해야 한다.
	mapRooms[roomKeys[0]]->SetRoomType(eRoomType::Start);
	mapRooms[roomKeys[1]]->SetRoomType(eRoomType::NextLevel);

	//const float battleRoomRatio = 0.7f;
	//테스트..
	//const float battleRoomRatio = 0.f;
	const float battleRoomRatio = 1.f;

	//나머지 방들은 비율에 맞춰 보물창고 / 전투 타입으로 지정한다.
	const auto& middleRoomStart = roomKeys.begin() + 2;
	const auto& middleRoomEnd = roomKeys.end();
	const int middleRoomLength = static_cast<int>(middleRoomEnd - middleRoomStart);
	const auto& treasureRoomStart = middleRoomStart + static_cast<int>(static_cast<float>(middleRoomLength) * battleRoomRatio);

	//전투 방 지정
	for (auto iterBattleRoom = middleRoomStart; iterBattleRoom != treasureRoomStart; ++iterBattleRoom)
	{
		mapRooms[*iterBattleRoom]->SetRoomType(eRoomType::Battle);
	}

	//아이템 방 지정
	for (auto iterTreasureRoom = treasureRoomStart; iterTreasureRoom != middleRoomEnd; ++iterTreasureRoom)
	{
		mapRooms[*iterTreasureRoom]->SetRoomType(eRoomType::Treasure);
	}
}

void TilemapLevel::RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile)
{
	if (!actorOnTile)
	{
		return;
	}

	const Vector2Int& newWorldPosition = actorOnTile->GetWorldPosition();
	RegisterActorOnTilemap(actorOnTile, newWorldPosition);
}

void TilemapLevel::RegisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Vector2Int& position)
{
	if (!actorOnTile)
	{
		return;
	}

	/* 해당 위치 타일의 Actor 리스트에 대상 Actor를 추가한다. */
	auto& actorListOnTile = mapActorListOnTilemap[position];
	actorListOnTile.emplace_back(actorOnTile);
}

void TilemapLevel::UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile)
{
	if (!actorOnTile)
	{
		return;
	}

	UnregisterActorOnTilemap(actorOnTile, actorOnTile->GetWorldPosition());
}

void TilemapLevel::UnregisterActorOnTilemap(std::shared_ptr<ActorOnTile> actorOnTile, const Vector2Int& position)
{
	if (!actorOnTile)
	{
		return;
	}

	const auto& findActorListOnTile = mapActorListOnTilemap.find(position);
	if (findActorListOnTile == mapActorListOnTilemap.end())
	{
		return;
	}

	auto& actorListOnTile = findActorListOnTile->second;

	auto remove_pred = [&actorOnTile](const std::weak_ptr<Actor>& weakPtr)
		{
			auto ptr = weakPtr.lock();

			//만료되었거나 대상 액터와 포인터 주소가 같은 경우 제거 대상
			return weakPtr.lock() == actorOnTile;
		};

	actorListOnTile.erase(std::remove_if(actorListOnTile.begin(), actorListOnTile.end(), remove_pred), actorListOnTile.end());
	if (actorListOnTile.empty())
	{
		/* Actor 리스트가 비었으면 map에서도 제거해준다. */
		mapActorListOnTilemap.erase(position);
	}
}

void TilemapLevel::OnMovePlayerEvent(const Vector2Int& prevWorldPosition, const Vector2Int& worldPosition)
{
	if (!tileMap)
	{
		return;
	}

	const UNIQUE_INDEX_TYPE prevRoomIndex = tileMap->GetTileRoomIndex(prevWorldPosition);
	const UNIQUE_INDEX_TYPE currentRoomIndex = tileMap->GetTileRoomIndex(worldPosition);

	/* 이전과 현재 위치해 있는 방이 서로 다른 경우 이벤트 발생 */
	if (prevRoomIndex != currentRoomIndex)
	{
		/* 현재 위치의 방 인덱스가 유효 => 특정 방에 진입했다는 것을 의미 */
		if (currentRoomIndex != ROOM_INDEX_INVALID)
		{
			if (onPlayerVisitedRoom)
			{
				RoomMapType::iterator iterRoom = mapRooms.find(currentRoomIndex);
				assert(iterRoom != mapRooms.end() && "Invalid iterator Room");

				const Room* findRoom = iterRoom->second.get();
				assert(findRoom && "Invalid Room");

				onPlayerVisitedRoom(*findRoom, worldPosition);
			}
		}
		else if (prevRoomIndex != ROOM_INDEX_INVALID && currentRoomIndex == ROOM_INDEX_INVALID)
		{
			/* 이전 위치의 방 인덱스가 유효했으나 현재 유효하지않으면 => 특정 방에서 빠져나왔다는것을 의미 */
			if (onPlayerLeavedRoom)
			{
				RoomMapType::iterator iterRoom = mapRooms.find(prevRoomIndex);
				assert(iterRoom != mapRooms.end() && "Invalid iterator Room");

				const Room* findRoom = iterRoom->second.get();
				assert(findRoom && "Invalid Room");

				onPlayerLeavedRoom(*findRoom, worldPosition);
			}
		}
	}
}
