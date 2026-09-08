#include "ProjectileFireBall.h"
#include "Actor/Pawn/Pawn.h"
#include "Actor/FX/FXExplosionActor.h"
#include "Level/TilemapLevel.h"
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <cassert>

using namespace Craft;

ProjectileFireBall::ProjectileFireBall(const Vector2Int& inPosition, 
										const float moveSpeed, 
										const eTeamID teamID, 
										float damageValue,
										float explosionRange,
										float explosionDamage)
	:super(inPosition, moveSpeed, teamID, damageValue)
	,explosionRange(explosionRange)
	,explosionDamage(explosionDamage)
{
	SetImage(L"●");
	SetImageColor(Color::Red);

	SetLifeSpan(1.f);
}

bool ProjectileFireBall::OnBlockWall(const Vector2Int& blockPosition, const Craft::Vector2Int& lastNonBlockPosition)
{
	if (!super::OnBlockWall(blockPosition, lastNonBlockPosition))
	{
		return false;
	}

	ApplyExplosionDamage(lastNonBlockPosition);
	SpawnExplosionFX(lastNonBlockPosition);

	return true;
}

bool ProjectileFireBall::OnBlockActor(std::shared_ptr<ActorOnTile> blockingActor, const Vector2Int& blockPosition, const Craft::Vector2Int& lastNonBlockPosition)
{
	if (!super::OnBlockActor(blockingActor, blockPosition, lastNonBlockPosition))
	{
		return false;
	}

	ApplyExplosionDamage(blockPosition);
	SpawnExplosionFX(blockPosition);

	return true;
}

void ProjectileFireBall::ApplyExplosionDamage(const Craft::Vector2Int& centerPosition)
{
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	const Vector2Int& centerPos = GetWorldPosition(); 
	const eTeamID instigatorTeamID = GetInstigatorTeamID();

	/* 미드포인트 원 알고리즘으로 타일범위들을 구함 */
	int minX = INT_MAX, maxX = INT_MIN;
	int minY = INT_MAX, maxY = INT_MIN;
	StaticFunctionLibrary::MidpointCircleProcess(
		centerPos,
		static_cast<int>(explosionRange),
		static_cast<int>(explosionRange * 0.6f),
		[&](int x, int y)
		{
			minX = min(minX, x);
			maxX = max(maxX, x);
			minY = min(minY, y);
			maxY = max(maxY, y);
		}
	);

	auto checkInsideEllipse = [](const Vector2Int& centerPos, const Vector2Int& checkPos, float xRadiusSq, float yRadiusSq)
		{
			const Vector2Int distance = checkPos - centerPos;
			const float distanceXSq = static_cast<float>(distance.x * distance.x);
			const float distanceYSq = static_cast<float>(distance.y * distance.y);

			const float ellipseValue = distanceXSq / xRadiusSq + distanceYSq / yRadiusSq;

			return ellipseValue <= 1.f;
		};

	float currentXRadiusSq = (explosionRange * explosionRange), currentYRadiusSq = (currentXRadiusSq * 0.6f * 0.6f);
	for (int yPos = minY; yPos <= maxY; ++yPos)
	{
		for (int xPos = minX; xPos <= maxX; ++xPos)
		{
			const Vector2Int checkTileCoord(xPos, yPos);

			/* 현재 범위 안에 들지 않았거나, 이전 범위 안에 들어 있던 좌표는 지나감 */
			if (!checkInsideEllipse(centerPos, checkTileCoord, currentXRadiusSq, currentYRadiusSq))
			{
				continue;
			}

			std::shared_ptr<Pawn> targetPawn = Cast<Pawn>(tilemapLevel->GetActorOnTileCoordinate(
				checkTileCoord,
				[&](std::weak_ptr<ActorOnTile> checkActor)
				{
					std::shared_ptr<Pawn> otherPawn = Cast<Pawn>(checkActor.lock());

					/* Pawn이 유효하지 않으면 건너뜀 */
					if (!otherPawn || otherPawn->HasExpired())
					{
						return false;
					}

					/* 죽은 Pawn은 처리하지 않음 */
					if (otherPawn->IsDeath())
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

			targetPawn->TakeDamage(explosionDamage);
			setDamagedPawns.insert(targetPawn->GetUniqueID());
		}
	}
}

void ProjectileFireBall::SpawnExplosionFX(const Vector2Int& spawnPosition)
{
	std::shared_ptr<Level> ownerLevel = GetOwner();
	assert(ownerLevel && "Invalid ownerLevel");

	ownerLevel->SpawnActor<FXExplosionActor>(spawnPosition, explosionRange, 2.f);
}
