#include "AbilityNovaActor.h"
#include "Types/Enums.h"
#include "Level/TilemapLevel.h"
#include "Actor/Pawn/Pawn.h"
#include <Render/Renderer.h>
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <cassert>

using namespace Craft;

AbilityNovaActor::AbilityNovaActor(const Craft::Vector2Int& position, 
									eTeamID TeamID, 
									float extendSpeed, 
									float beginRange, 
									float limitRange, 
									float damage)
	:super(position)
	,instigatorTeamID(TeamID)
	,extendSpeed(extendSpeed)
	,currentRange(beginRange)
	,limitRange(limitRange)
	,damageAmount(damage)
{
	
}

void AbilityNovaActor::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	UpdateRangeCircle(deltaTime);
	
	if (currentRange >= limitRange)
	{
		/* 제한 범위를 넘어서면 Destory */
		Destroy();
	}
}

void AbilityNovaActor::PostTick(float deltaTime)
{
	super::PostTick(deltaTime);

	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	/* 타일 내 위치한 Pawn들에 대한 데미지 처리 */
	auto DamageProcessOnTile = [&](const Vector2Int& tileCoord)
		{
			if (!IsValidTileCoordInCircle(*tilemapLevel, tileCoord))
			{
				return;
			}

			/* 타일 위에 위치한 Pawn을 가져온다. */
			std::shared_ptr<Pawn> pawnOnTile = tilemapLevel->GetPawnOnTile(tileCoord);
			if (!pawnOnTile || pawnOnTile->GetTeamID() == instigatorTeamID)
			{
				/* 타일 위에 있는 폰이 없거나, 같은 팀이면 데미지 처리하지 않음 */
				return;
			}

			const PawnUniqueIDType pawnUniqueID = pawnOnTile->GetUniqueID();

			/* 이미 데미지를 준 대상이면 처리하지 않음 */
			if (setDamagedPawns.find(pawnUniqueID) != setDamagedPawns.end())
			{
				return;
			}

			/* 범위 내 적대대상 Pawn에게 데미지 적용 */
			setDamagedPawns.insert(pawnUniqueID);
			pawnOnTile->TakeDamage(damageAmount);
		};

	/* 내부 타일에 대한 데미지 처리 */
	for (const Vector2Int& innerTileCoord : circleInnerTileCoord)
	{
		DamageProcessOnTile(innerTileCoord);
	}

	/* 외곽 타일에 대한 데미지 처리 */
	for (const Vector2Int& outerTileCoord : circleOuterTileCoord)
	{
		DamageProcessOnTile(outerTileCoord);
	}
}

void AbilityNovaActor::Draw()
{
	super::Draw();

	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	Renderer& renderer = Renderer::Get();
	for (const Vector2Int& tileCoord : circleOuterTileCoord)
	{
		if (!IsValidTileCoordInCircle(*tilemapLevel, tileCoord))
		{
			continue;
		}

		renderer.Submit(outlineImage, tileCoord, outlineColor, static_cast<int>(eRenderSortingOrder::beam));
	}
}

void AbilityNovaActor::UpdateRangeCircle(float deltaTime)
{
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	/* 진행 시간에 따라 범위를 늘려 준다. */
	currentRange = min(limitRange, currentRange + (extendSpeed * deltaTime));
	
	/* BFS 탐색으로 유효한 노바 범위 내 타일 위치 구하기 */
	circleOuterTileCoord.clear();
	circleInnerTileCoord.clear();
	tilemapLevel->GetAvailableTilesInRange(shared_from_this(), static_cast<int>(currentRange), circleOuterTileCoord, circleInnerTileCoord);
}

bool AbilityNovaActor::IsValidTileCoordInCircle(const TilemapLevel& level, const Vector2Int& tileCoord) const
{
	if (eTileCategory::Ground != level.GetTileCategory(tileCoord))
	{
		return false;
	}

	return true;
}
