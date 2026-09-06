#include "AbilityShockWaveActor.h"
#include "Level/TilemapLevel.h"
#include "Actor/Pawn/Pawn.h"
#include <Render/Renderer.h>
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <Math/Define.h>
#include <cassert>

using namespace Craft;

AbilityShockWaveActor::AbilityShockWaveActor(const Vector2Int& position,
											eTeamID TeamID,
											float extendSpeed,
											float beginRange,
											float limitRange,
											float duration,
											float damage)
	:super(position)
	,instigatorTeamID(TeamID)
	,extendSpeed(extendSpeed)
	,currentRange(beginRange)
	,limitRange(limitRange)
	,duration(duration)
	,damageAmount(damage)
{

}

void AbilityShockWaveActor::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	UpdateRangeCircle(deltaTime);

	if (elapsedTime >= duration)
	{
		/* 시간 초과하면 Destroy */
		Destroy();
	}
}

void AbilityShockWaveActor::PostTick(float deltaTime)
{
	super::PostTick(deltaTime);

	ApplyRangeDamage();
}

void AbilityShockWaveActor::Draw()
{
	super::Draw();

	Renderer& renderer = Renderer::Get();
	for (const Vector2Int& tileCoord : circleOuterTileCoord)
	{
		renderer.Submit(outlineImage, tileCoord, outlineColor, static_cast<int>(eRenderSortingOrder::beam));
	}
}

void AbilityShockWaveActor::UpdateRangeCircle(float deltaTime)
{
	/* 진행 시간에 따라 범위를 늘려 준다. */
	elapsedTime += deltaTime;
	elapsedTime = min(elapsedTime, duration);
	const float t = elapsedTime / duration;
	const float acceleratedT = pow(t, 1.5f);
	currentRange = limitRange * acceleratedT;

	/* 미드포인트 원 알고리즘으로 타원 형태의 외곽 타일을 구함 */
	int minX = INT_MAX, maxX = INT_MIN;
	int minY = INT_MAX, maxY = INT_MIN;
	circleOuterTileCoord.clear();
	StaticFunctionLibrary::MidpointCircleProcess(
		GetWorldPosition(), 
		static_cast<int>(currentRange), 
		static_cast<int>(currentRange * yRadiusRate),
		[&](int x, int y)
		{
			circleOuterTileCoord.push_back(Vector2Int(x, y));

			minX = min(minX, x);
			maxX = max(maxX, x);
			minY = min(minY, y);
			maxY = max(maxY, y);
		}
	);

	minTileCoord = Vector2Int(minX, minY);
	maxTileCoord = Vector2Int(maxX, maxY);
}

void AbilityShockWaveActor::ApplyRangeDamage()
{
	/* Min, Max 범위 검사 */
	if (minTileCoord.y > maxTileCoord.y || minTileCoord.x > maxTileCoord.x)
	{
		return;
	}

	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	const Vector2Int& centerPos = GetWorldPosition();
	float prevXRadiusSq = (prevRange * prevRange), prevYRadiusSq = (prevXRadiusSq * yRadiusRate * yRadiusRate);
	float currentXRadiusSq = (currentRange * currentRange), currentYRadiusSq = (currentXRadiusSq * yRadiusRate * yRadiusRate);

	auto checkInsideEllipse = [](const Vector2Int& centerPos, const Vector2Int& checkPos, float xRadiusSq, float yRadiusSq)
		{
			const Vector2Int distance = checkPos - centerPos;
			const float distanceXSq = static_cast<float>(distance.x * distance.x);
			const float distanceYSq = static_cast<float>(distance.y * distance.y);

			const float ellipseValue = distanceXSq / xRadiusSq + distanceYSq / yRadiusSq;

			return ellipseValue <= 1.f;
		};
	
	for (int yPos = minTileCoord.y; yPos <= maxTileCoord.y; ++yPos)
	{
		for (int xPos = minTileCoord.x; xPos <= maxTileCoord.x; ++xPos)
		{
			const Vector2Int checkTileCoord(xPos, yPos);

			/* 현재 범위 안에 들지 않았거나, 이전 범위 안에 들어 있던 좌표는 지나감 */
			if (!checkInsideEllipse(centerPos, checkTileCoord, currentXRadiusSq, currentYRadiusSq) ||
				checkInsideEllipse(centerPos, checkTileCoord, prevXRadiusSq, prevYRadiusSq))
			{
				continue;
			}

			/* prev, Current 타원 범위안의 대상을 찾아서 데미지를 주어야 함*/
			std::shared_ptr<Pawn> targetPawn = Cast<Pawn>(tilemapLevel->GetActorOnTileCoordinate(
				checkTileCoord,
				[&](std::weak_ptr<ActorOnTile> checkActor)
				{
					std::shared_ptr<Pawn> otherPawn = Cast<Pawn>(checkActor.lock());

					/* Pawn이 유효하지 않으면 건너뜀 */
					if (!otherPawn || 
						otherPawn->HasExpired())
					{
						return false;
					}

					/* 같은 팀 소속, 이미 데미지를 준 상대면 건너뜀 */
					if (otherPawn->GetTeamID() == instigatorTeamID ||
						setDamagedPawns.find(otherPawn->GetUniqueID()) != setDamagedPawns.end())
					{
						return false;
					}

					return true;
				}
			));	

			if (!targetPawn)
			{
				continue;
			}

			targetPawn->TakeDamage(damageAmount);
			setDamagedPawns.insert(targetPawn->GetUniqueID());
		}
	}

	prevRange = currentRange;
}
