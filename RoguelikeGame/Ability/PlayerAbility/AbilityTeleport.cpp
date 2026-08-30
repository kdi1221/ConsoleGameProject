#include "AbilityTeleport.h"
#include "Actor/Pawn/Pawn.h"
#include "Game/State/GameMode/GM_Roguelike.h"
#include "Level/TilemapLevel.h"
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <Math/Vector2Float.h>
#include <Engine/Engine.h>
#include <cassert>


using namespace Craft;

AbilityTeleport::AbilityTeleport(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{
	SetCooldownTime(0.5f);
	SetManaCost(5.f);
}

void AbilityTeleport::ActivateAbility()
{
	super::ActivateAbility();

	GM_Roguelike* gameMode = Engine::Get().GetGameMode<GM_Roguelike>();
	assert(gameMode && "Invalid gameMode");

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invalid ownerActor");

	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(ownerPawn->GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	/* owner의 조준 방향 */
	const Vector2Float aimingDirection = ownerPawn->GetAimingDirection();
	assert(aimingDirection != Vector2Float::Zero && "aimingDirection is zero..");

	/* 이동 전 위치 */
	const Vector2Int& prevPosition = ownerPawn->GetWorldPosition();
	const RoomDefines::UNIQUE_INDEX_TYPE ownerRoomIndex = tilemapLevel->GetRoomIndexInTile(prevPosition);

	/* 텔레포트 목표 지점 */
	const Vector2Int destinationPosition = prevPosition + static_cast<Vector2Int>(aimingDirection * maxRange);

	//브레젠험 직선그리기로 도착지점까지의 경로 타일들을 구한다.
	std::vector<Vector2Int> pathTiles;
	StaticFunctionLibrary::GetBresenhamPath(prevPosition, destinationPosition, pathTiles);
	assert(!pathTiles.empty() && "pathTiles empty..");

	//브레젠험으로 구한 경로의 첫 시작은 현재타일위치와 같음
	auto iterMoveNextTileCoord = pathTiles.begin();

	//마지막으로 이동가능이 확인된 타일 위치
	auto iterlastMoveEnableTileCoord = iterMoveNextTileCoord;

	//시작 바로 다음 경로부터 체크 시작
	++iterMoveNextTileCoord;

	/* 각 경로타일에서 막히는 경로가 있는지(벽, 다른 Pawn) 확인해서 블록된 경로 바로 앞부분을 이동위치로 삼는다. */
	for (; iterMoveNextTileCoord != pathTiles.end(); ++iterMoveNextTileCoord)
	{
		const Vector2Int& checkTileCoord = *iterMoveNextTileCoord;

		/* 해당 경로가 벽이거나 누가 점유중이면 이동 불가 */
		if (CheckPlacementResult::CanMove != tilemapLevel->CanNextMove(ownerPawn, checkTileCoord))
		{
			continue;
		}

		/* 플레이어가 전투방 내에서 전투중이면 방 내의 동일한 타일에서만 이동 가능 */
		if (gameMode->IsBattleRoomProcess())
		{
			if (ownerRoomIndex != tilemapLevel->GetRoomIndexInTile(checkTileCoord))
			{
				continue;
			}
		}

		/* 마지막으로 이동가능이 확인된 타일 위치 저장 */
		iterlastMoveEnableTileCoord = iterMoveNextTileCoord;
	}


	/* Owner Pawn의 위치 이동 */
	ownerPawn->SetPosition(*iterlastMoveEnableTileCoord);
	
	//TODO : 소환수들도 같이 움직여야 함



	/* 사운드 재생 */
	Engine::Get().PlayOneShot("Effect/teleport.wav");

	EndAbility(false);
}
